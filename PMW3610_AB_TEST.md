# PMW3610 A/B firmware test

This branch freezes the source revisions used by the successful firmware build
on 2026-04-21 (GitHub Actions run `24703904180`) and builds controlled
right-half firmware variants.

| Artifact | PMW3610 implementation | Smart mode | Purpose |
| --- | --- | --- | --- |
| `charybdis-right-badjeff-pinned.uf2` | badjeff `44b4a76b...` | On | Rollback control |
| `charybdis-right-official-invert-xy-no-swap-pinned.uf2` | upstream Zephyr driver | On | Official-driver baseline with verified axes |
| `charybdis-right-official-invert-xy-no-swap-smart-off-pinned.uf2` | upstream Zephyr driver | Off | One-variable smart-mode test |

Pinned source revisions:

- ZMK: `0331b7d16e80954b807917f9323e59ffc1e3b626`
- badjeff PMW3610: `44b4a76b74d293a93cec4ccb7e04cb8d29c10f93`
- urob auto-layer: `dfa7c695ffde41ef8064245453595d50e198ae1d`
- keyboard config: `b98cb3e95105939e7fd9fdd5a479056b2abb428a`

## Safe test order

1. Keep `charybdis-right-badjeff-pinned.uf2` as the rollback image.
2. Power the right half from a stable source while flashing.
3. Keep `charybdis-right-official-invert-xy-no-swap-pinned.uf2` as the
   official-driver smart-on baseline.
4. Flash only
   `charybdis-right-official-invert-xy-no-swap-smart-off-pinned.uf2` to the
   right half.
5. Do not flash `settings_reset` and do not clear Bluetooth pairings for this test.
6. Confirm pointer direction, scrolling, and clicks, then observe normal use for several days.
7. If tracking quality is worse, restore the smart-on official baseline. If the
   whole-keyboard freeze recurs, classify smart mode as insufficient and restore
   the smart-on baseline before testing report-rate limits.

The driver variants share the board target, ZMK/Zephyr version, keymap, BLE
setup, 400 CPI, and scroll scaling. The official variants use the upstream
driver's `invert-x` and `invert-y` properties but do not swap the axes or apply
a ZMK coordinate processor. This follows on-device tests showing that
horizontal and vertical axes are aligned while both directions require
reversal.

The smart-on official baseline reproduced a whole-central/BLE freeze during
active trackball use. A single physical reset restored the existing Bluetooth
pairing in approximately 1–3 seconds, which makes a persistent loss of power
unlikely. The smart-off variant changes only the `smart-mode` devicetree
property; CPI, axes, keymap, source revisions, BLE configuration, and reporting
path remain unchanged.
