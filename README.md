## Interactive Game of Life Implementation in C using Raylib.

This is an implementation of [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) written in C, using [Raylib](https://www.raylib.com/) for the graphics.
The canvas is initialised with a simple [Glider](https://conwaylife.com/wiki/Glider), or with a saved state from the file argument.

### TODO:

- [x] Load state from file
- [x] Save state to file
- [x] Show events notification (e.g. on file save or error)
- [ ] Allow mouse dragging in DRAW mode (?)

### Compile and Run:

```console
make
./gol ["state.gol"]
```

Before compiling, install [Raylib](https://www.raylib.com/).

### Controls

- (d): enter DRAW mode and use mouse to spawn/kill cells on the canvas
- (p): save current state in a checkpoint and PLAY the simulation
- (w): REWIND to the state saved in the checkpoint
- (r): RESET to an empty canvas
- (S): SAVE the current state to a file (`state.gol`) to load it in a future session

### Video example:

[screencast.webm](https://github.com/user-attachments/assets/9cd04de2-3ed5-46ea-8c56-138ceea12eeb)
