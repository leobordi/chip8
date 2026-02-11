# CHIP 8 EMULATOR 🚀

Emulatore di chip8 scritto in c.

## Review Issues

#### Critical / correctness

- Missing opcode implementations: `BNNN` and `CXNN` are decoded but not executed; `0NNN` is decoded but mapped to `IN_NONE` and never executed. This breaks ROMs that use them.
- Incorrect font address resolution for `FX29`: `get_font_address()` scans memory for a byte equal to the font index, which can return arbitrary addresses. Correct address is `0x50 + (font * 5)`.
- ROM load can overflow RAM: `chip8_load_rom()` reads the full file into memory without checking `RAM_SIZE - FIRST_INST`.
- Stack underflow/overflow not checked: `IN_2NNN` increments `sp` and `IN_00EE` decrements it with no bounds checks.
- Program counter bounds not checked: `fetch()` reads two bytes without ensuring `pc + 1 < RAM_SIZE`.

#### Behavior / compatibility

- Sprite drawing clips at edges (no wrap): `DXYN` stops at screen edges. Many ROMs expect wraparound; should be configurable if targeting modern/compat modes.
- `FX1E` does not set `VF` on overflow: some interpreters set `VF` when `I` exceeds 0xFFF.
- `FX0A` key wait logic is unusual: it defers storing until release; typical behavior stores on key press. Can cause timing quirks.

#### Resource management / SDL

- Renderer/texture leaks: `display_close()` destroys only the window; no `SDL_DestroyRenderer()` or `SDL_DestroyTexture()`.
- Audio shutdown not guaranteed if `display_close()` is called without prior `audio_close()`.

#### Input / portability

- Hardcoded scancodes in `get_key()` are fragile across platforms/layouts. Prefer `SDL_Scancode` constants.

#### Timing / performance

- No CPU throttle: main loop runs as fast as possible, which makes ROMs run too fast on modern CPUs.
- Timer tick update can drift: `get_elapsed_ticks()` sets `last_timer_update` to `current_time` when ticks >= 1, losing accumulated ticks during stalls.

#### Lower priority

- `keyboard_init()` does not clear `buffer` (relies on global zero-init). Re-init could keep stale state.
- Fixed non-integer window scaling (1200x720 for 64x32) can cause uneven pixel scaling.

## Installation and setup

## Dependecies

* SDL2
