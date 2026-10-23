# Rub-Da-Rabbit

A small 2D arcade game built with C++17 and SDL2. You play as a rabbit that
collects carrots while dodging enemies that chase you. The more carrots you
grab, the higher your score — but every carrot also spawns another enemy.

## Features

- Collect carrots to raise your score (each carrot is worth 1 point).
- Enemies chase you and end the game on contact.
- A stamina system: jumping costs stamina, and collecting carrots restores it.
- Persistent high score, saved to `save.dat` next to the executable.
- 1600×900 resizable window with a fixed logical resolution.

## Controls

| Key       | Action                  |
| --------- | ----------------------- |
| `W` / `S` | Move up / down          |
| `A` / `D` | Move left / right       |
| `Space`   | Jump (costs 1 stamina)  |
| `Enter`   | Restart after game over |

## Dependencies

- A C++17 compiler (`g++` or `clang++`)
- [SDL2](https://www.libsdl.org/)
- [SDL2_image](https://www.libsdl.org/projects/SDL_image/)
- [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/)
- [SDL2_mixer](https://www.libsdl.org/projects/SDL_mixer/)

`make` (Linux/macOS) or `mingw32-make` (Windows) is needed for the Makefile build.
The platform is detected automatically, or can be selected explicitly with
`PLATFORM=linux` or `PLATFORM=windows`.

## Building

### Linux (Debian/Ubuntu)

```sh
sudo apt update
sudo apt install g++ make libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev
make                  # builds bin/rubdarabbits
make PLATFORM=linux   # explicitly select the Linux build
make run              # builds and runs
```

### Windows (MSYS2 / MinGW)

Install MSYS2 and open an **MSYS2 MINGW64** shell, then:

```sh
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_ttf mingw-w64-x86_64-SDL2_mixer mingw-w64-x86_64-make
mingw32-make                    # builds bin/rubdarabbits.exe
mingw32-make PLATFORM=windows  # explicitly select the Windows build
mingw32-make run                # builds and runs
```

### Manual build (no Makefile)

Linux/macOS:

```sh
g++ -std=c++17 src/*.cpp -o bin/rubdarabbits -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
```

Windows (MinGW):

```sh
g++ -std=c++17 src/*.cpp -o bin/rubdarabbits -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
```

### Other targets

- `make debug` — build with `-g -O0` (run `make clean && make debug` to force a
  rebuild with debug flags).
- `make clean` — remove `build/` and the compiled binary.

## Project structure

```
Rub-Da-Rabbit/
├── assets/        # images and fonts
│   ├── carrot.png
│   ├── enemy.png
│   ├── player.png
│   └── font.ttf
├── src/           # source code
│   ├── main.cpp
│   ├── game.{h,cpp}
│   ├── player.{h,cpp}
│   ├── enemy.{h,cpp}
│   ├── ui.{h,cpp}
│   └── consts.h
├── bin/           # compiled binary output
├── Makefile
└── README.md
```

## Notes

- The high score is stored in `save.dat` in the working directory. Delete this
  file to reset it.
- `save.dat` and the compiled binary in `bin/` are currently tracked by git; if
  you want them ignored and untracked, run
  `git rm --cached save.dat bin/rubdarabbits`.
- The window title is "Rub da Rabbits".
