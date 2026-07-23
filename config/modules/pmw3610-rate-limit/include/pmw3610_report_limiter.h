/*
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <zephyr/device.h>
#include <zephyr/kernel.h>

int pmw3610_rate_limited_input_report_rel(const struct device *dev, uint16_t code,
                                          int32_t value, bool sync,
                                          k_timeout_t timeout);
