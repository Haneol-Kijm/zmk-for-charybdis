# PMW3610 A/B firmware test

This branch freezes the source revisions used by the successful firmware build
on 2026-04-21 (GitHub Actions run `24703904180`) and builds two right-half
firmware variants.

| Artifact | PMW3610 implementation | Purpose |
| --- | --- | --- |
| `charybdis-right-badjeff-pinned.uf2` | badjeff `44b4a76b...` | Control: reproduces the current source configuration |
| `charybdis-right-official-invert-xy-no-swap-pinned.uf2` | upstream Zephyr driver | Test: inverts both directions without swapping the axes |

Pinned source revisions:

- ZMK: `0331b7d16e80954b807917f9323e59ffc1e3b626`
- badjeff PMW3610: `44b4a76b74d293a93cec4ccb7e04cb8d29c10f93`
- urob auto-layer: `dfa7c695ffde41ef8064245453595d50e198ae1d`
- keyboard config: `b98cb3e95105939e7fd9fdd5a479056b2abb428a`

## Safe test order

1. Keep `charybdis-right-badjeff-pinned.uf2` as the rollback image.
2. Power the right half from a stable source while flashing.
3. Flash only `charybdis-right-official-invert-xy-no-swap-pinned.uf2` to the right half.
4. Do not flash `settings_reset` and do not clear Bluetooth pairings for this test.
5. Confirm pointer direction, scrolling, and clicks, then observe normal use for several days.
6. If initialization, tracking, or disconnect behavior is worse, flash the pinned badjeff image back to the right half.

The two variants share the board target, ZMK/Zephyr version, keymap, BLE setup,
400 CPI, and scroll scaling. The official variant uses the upstream driver's
`invert-x` and `invert-y` properties but does not swap the axes or apply a ZMK
coordinate processor. This follows on-device tests showing that horizontal and
vertical axes are aligned while both directions require reversal.
Driver-specific initialization, power management, and reporting behavior
remain intentionally different.
