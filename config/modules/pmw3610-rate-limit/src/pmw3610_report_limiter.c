/*
 * SPDX-License-Identifier: MIT
 */

#include <stdbool.h>
#include <stdint.h>

#include <zephyr/dt-bindings/input/input-event-codes.h>
#include <zephyr/input/input.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>

#include <pmw3610_report_limiter.h>

struct pmw3610_report_limiter_state {
    const struct device *dev;
    int32_t x;
    int32_t y;
    int64_t last_report_ms;
    bool have_x;
    bool have_y;
    bool report_scheduled;
};

static struct pmw3610_report_limiter_state state;

static int flush_report(void) {
    const struct device *dev = state.dev;
    const int32_t x = state.x;
    const int32_t y = state.y;
    const bool have_x = state.have_x;
    const bool have_y = state.have_y;
    int ret = 0;

    state.x = 0;
    state.y = 0;
    state.have_x = false;
    state.have_y = false;
    state.last_report_ms = k_uptime_get();

    if (have_x) {
        ret = input_report_rel(dev, INPUT_REL_X, x, !have_y, K_NO_WAIT);
    }

    if (have_y) {
        const int y_ret = input_report_rel(dev, INPUT_REL_Y, y, true, K_NO_WAIT);

        if (ret == 0) {
            ret = y_ret;
        }
    }

    return ret;
}

static void report_work_handler(struct k_work *work) {
    ARG_UNUSED(work);

    state.report_scheduled = false;
    flush_report();
}

K_WORK_DELAYABLE_DEFINE(report_work, report_work_handler);

int pmw3610_rate_limited_input_report_rel(const struct device *dev, uint16_t code,
                                          int32_t value, bool sync,
                                          k_timeout_t timeout) {
    if (code != INPUT_REL_X && code != INPUT_REL_Y) {
        return input_report_rel(dev, code, value, sync, timeout);
    }

    state.dev = dev;

    if (code == INPUT_REL_X) {
        state.x += value;
        state.have_x = true;
    } else {
        state.y += value;
        state.have_y = true;
    }

    if (!sync) {
        return 0;
    }

    const int64_t elapsed_ms = k_uptime_get() - state.last_report_ms;

    if (state.last_report_ms == 0 ||
        elapsed_ms >= CONFIG_PMW3610_RATE_LIMIT_INTERVAL_MS) {
        if (state.report_scheduled) {
            k_work_cancel_delayable(&report_work);
            state.report_scheduled = false;
        }

        return flush_report();
    }

    if (!state.report_scheduled) {
        const int64_t delay_ms =
            CONFIG_PMW3610_RATE_LIMIT_INTERVAL_MS - elapsed_ms;

        state.report_scheduled = true;
        k_work_schedule(&report_work, K_MSEC(delay_ms));
    }

    return 0;
}
