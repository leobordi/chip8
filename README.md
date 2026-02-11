# CHIP 8 EMULATOR 🚀

Emulatore di chip8 scritto in c.

## Review Issues

#### Critical / correctness

- Missing opcode implementations: `BNNN` and `CXNN` are decoded but not executed; `0NNN` is decoded but mapped to `IN_NONE` and never executed. This breaks ROMs that use them.
  - Files: `src/lib/core.c`

- Incorrect font address resolution for `FX29`: `get_font_address()` scans memory for a byte equal to the font index, which can return arbitrary addresses. Correct address is `0x50 + (font * 5)`.
  - Files: `src/lib/core.c`

- ROM load can overflow RAM: `chip8_load_rom()` reads the full file into memory without checking `RAM_SIZE - FIRST_INST`.
  - Files: `src/lib/core.c`

- Stack underflow/overflow not checked: `IN_2NNN` increments `sp` and `IN_00EE` decrements it with no bounds checks.
  - Files: `src/lib/core.c`

- Program counter bounds not checked: `fetch()` reads two bytes without ensuring `pc + 1 < RAM_SIZE`.
  - Files: `src/lib/core.c`

#### Behavior / compatibility

- Sprite drawing clips at edges (no wrap): `DXYN` stops at screen edges. Many ROMs expect wraparound; should be configurable if targeting modern/compat modes.
  - Files: `src/lib/core.c`

- `FX1E` does not set `VF` on overflow: some interpreters set `VF` when `I` exceeds 0xFFF.
  - Files: `src/lib/core.c`

- `FX0A` key wait logic is unusual: it defers storing until release; typical behavior stores on key press. Can cause timing quirks.
  - Files: `src/lib/core.c`

#### Resource management / SDL

- Renderer/texture leaks: `display_close()` destroys only the window; no `SDL_DestroyRenderer()` or `SDL_DestroyTexture()`.
  - Files: `src/lib/display.c`

- Audio shutdown not guaranteed if `display_close()` is called without prior `audio_close()`.
  - Files: `src/lib/display.c`, `src/lib/core.c`

#### Input / portability

- Hardcoded scancodes in `get_key()` are fragile across platforms/layouts. Prefer `SDL_Scancode` constants.
  - Files: `src/lib/keyboard.c`

#### Timing / performance

- No CPU throttle: main loop runs as fast as possible, which makes ROMs run too fast on modern CPUs.
  - Files: `src/lib/core.c`

- Timer tick update can drift: `get_elapsed_ticks()` sets `last_timer_update` to `current_time` when ticks >= 1, losing accumulated ticks during stalls.
  - Files: `src/lib/core.c`

#### Lower priority

- `keyboard_init()` does not clear `buffer` (relies on global zero-init). Re-init could keep stale state.
  - Files: `src/lib/keyboard.c`

- Fixed non-integer window scaling (1200x720 for 64x32) can cause uneven pixel scaling.
  - Files: `src/lib/display.c`

## Installation and setup

## Dependecies

* SDL2
