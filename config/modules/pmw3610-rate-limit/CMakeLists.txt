# SPDX-License-Identifier: MIT

if(CONFIG_INPUT_PMW3610_RATE_LIMITED)
  zephyr_library()
  zephyr_library_include_directories(
    include
    ${ZEPHYR_BASE}/drivers/input
  )
  zephyr_library_sources(
    src/input_pmw3610_rate_limited.c
    src/pmw3610_report_limiter.c
  )
endif()
