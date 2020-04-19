# Cetris

A (modern) Tetris clone written in C++ and SDL2.0 ([Download Here](https://github.com/Blubberpie/cetris/releases/latest))

## NOTE
**This is by no means an optimized game**, and since it's my first C++ program, I probably have unintentionally broken several conventions and best practices. This was purely meant as a fun coding challenge for me, and I'm just sharing it.

You'll also notice that the control scheme doesn't use the arrow keys for movement, but WASD. It's just what I'm comfortable with, since it very much resembles how you'd play on a controller.

## Main Features
- Delayed auto-shift (DAS) and auto repeat rate (ARR), to make gameplay feel better
- Lock delay (infinite)
- Super Rotation System (see https://tetris.wiki/Super_Rotation_System)
- Wall kicks (T-Spin as much as you want)
- BPS's Random Generator (see https://tetris.wiki/Random_Generator)
- Ghost tetrominos
- Texture-based rendering
- A basic scoring system (no combos or T-Spin detection)

## Features NOT Implemented
- Music
- T-Spin detection (T-Spins are **very** doable, but there's no reward)
- Garbage
- Levels
- Everything else

## Controls
- Left: A
- Down: S
- Right: D
- Hard Drop: W
- Hold: Q
- Rotate Left: Down Arrow
- Rotate Right: Right Arrow

Happy misdropping! :)
