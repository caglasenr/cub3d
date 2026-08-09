*This project has been created as part of the 42 curriculum by `iogul`, `csener`.*

# cub3D

## Description

**cub3D** is a first-person raycasting engine inspired by *Wolfenstein 3D* (1992), the game widely credited as the origin of the FPS genre. Built in C with the **MiniLibX** graphics library, the project recreates the classic technique of casting rays from the player's position to render a pseudo-3D view of a 2D maze in real time.

The goal is not just to draw a maze, but to do it *correctly and cleanly*: parsing a custom `.cub` scene file, validating it rigorously, and rendering a smooth, responsive window where the player can move and look around without crashes, leaks, or visual glitches.

**Core features:**
- Real-time raycasting engine rendering a first-person view of a maze
- Custom `.cub` scene file parser with strict format validation
- Four distinct wall textures (North, South, East, West)
- Configurable floor and ceiling colors
- Smooth player movement (`W`, `A`, `S`, `D`) and camera rotation (`←`, `→`)
- Clean exit on `ESC` or window close, with full memory management (no leaks)

## Instructions

### Requirements
- A C compiler (`cc`)
- `make`
- MiniLibX (system version or bundled sources)

### Compilation
```bash
make          # builds the mandatory part
make bonus    # builds the bonus part (if implemented)
make clean    # removes object files
make fclean   # removes object files and the binary
make re       # rebuilds from scratch
```

### Running
```bash
./cub3D <path_to_map.cub>
```

Example:
```bash
./cub3D maps/example.cub
```

### Controls
| Key | Action |
|---|---|
| `W` / `A` / `S` / `D` | Move through the maze |
| `←` / `→` | Look left / right |
| `ESC` | Quit |
| Window close button | Quit |

### Scene file format (`.cub`)
A valid scene file defines four textures, two colors, and a map:
```
NO ./textures/north.xpm
SO ./textures/south.xpm
WE ./textures/west.xpm
EA ./textures/east.xpm
F 220,100,0
C 225,30,0

111111
100101
101001
1100N1
111111
```
The map must be fully enclosed by walls (`1`), and the player's starting position and orientation is marked with `N`, `S`, `E`, or `W`. Any malformed file causes the program to exit cleanly with an explicit `Error\n` message.

## Resources

- [Lode's Computer Graphics Tutorial – Raycasting](https://lodev.org/cgtutor/raycasting.html) — the classic reference for the DDA raycasting algorithm
- [MiniLibX documentation](https://harm-smits.github.io/42docs/libs/minilibx) — window/event/image API reference
- [Wolfenstein 3D (original game)](http://users.atw.hu/wolf3d/) — the source of inspiration for this project
- 42 Network peer resources and internal documentation on raycasting and the Norm

**AI usage:** AI assistance was used selectively during development — primarily to clarify raycasting math (DDA algorithm, ray-direction vectors) and to review error-handling edge cases in the `.cub` parser. All AI-suggested logic was rewritten by hand, tested independently, and fully understood before being integrated, in line with the project's AI usage guidelines. No AI-generated code was copy-pasted directly into the submission.
