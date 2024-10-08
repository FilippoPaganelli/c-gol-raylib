## Interactive Game of Life Implementation in C using Raylib.

This is an implementation of [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) written in C, using [Raylib](https://www.raylib.com/) for the graphics.
The cells grid is initialised with a simple [Glider](https://conwaylife.com/wiki/Glider).

### TODO:

- [ ] Add preset patterns
- [ ] (?) Allow mouse dragging in DRAW mode

### Controls

By pressing **`d`** or **`p`**, you switch between **`DRAW`** and **`PLAY`** modes. In `DRAW` mode you can design your starting state, then let it simulate in `PLAY` mode.

By pressing **`r`**, you reset the grid state to blank.

Video example:

[c-gol-raylib-interactive.webm](https://github.com/user-attachments/assets/78b1f81f-25a3-47a1-ace7-630c7b1ca504)

Before compiling, install [Raylib](https://www.raylib.com/).

### Compile and Run:

```console
make && ./main
```
