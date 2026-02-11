# Chip-8 Emulator Review Issues

## Critical / correctnes

- Incorrect font address resolution for `FX29`: `get_font_address()` scans memory for a byte equal to the font index, which can return arbitrary addresses. Correct address is `0x50 + (font * 5)`.
  - Files: `src/lib/core.c`

## Behavior / compatibility

- Sprite drawing clips at edges (no wrap): `DXYN` stops at screen edges. Many ROMs expect wraparound; should be configurable if targeting modern/compat modes.
  - Files: `src/lib/core.c`

- `FX0A` key wait logic is unusual: it defers storing until release; typical behavior stores on key press. Can cause timing quirks.
  - Files: `src/lib/core.c`

## Input / portability

- Hardcoded scancodes in `get_key()` are fragile across platforms/layouts. Prefer `SDL_Scancode` constants.
  - Files: `src/lib/keyboard.c`

## Timing / performance

- No CPU throttle: main loop runs as fast as possible, which makes ROMs run too fast on modern CPUs.
  - Files: `src/lib/core.c`

- Timer tick update can drift: `get_elapsed_ticks()` sets `last_timer_update` to `current_time` when ticks >= 1, losing accumulated ticks during stalls.
  - Files: `src/lib/core.c`

## Lower priority

- Fixed non-integer window scaling (1200x720 for 64x32) can cause uneven pixel scaling.
  - Files: `src/lib/display.c`
