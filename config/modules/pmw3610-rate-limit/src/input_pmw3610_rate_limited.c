/*
 * SPDX-License-Identifier: MIT
 *
 * Compile the exact PMW3610 implementation from the pinned Zephyr tree while
 * replacing only its relative input-report calls with the local aggregator.
 */

#include <zephyr/input/input.h>

#include <pmw3610_report_limiter.h>

#define input_report_rel pmw3610_rate_limited_input_report_rel
#include "input_pmw3610.c"
