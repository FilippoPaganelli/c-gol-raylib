## Interactive Game of Life Implementation in C using Raylib.

This is an implementation of [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) written in C, using [Raylib](https://www.raylib.com/) for the graphics.
The canvas is initialised with a simple [Glider](https://conwaylife.com/wiki/Glider), or with a saved state from the file argument.

### TODO:

- [x] Load state from file
- [x] Save state to file
- [x] Show events notification (e.g. on file save or error)
- [x] Add key command to randomly generate a state
- [ ] Allow mouse dragging in DRAW mode (?)

### Compile and Run:

```console
make
./gol ["state.gol"]
```

Before compiling, install [Raylib](https://www.raylib.com/).

### Controls:

- (d): enter DRAW mode and use mouse to spawn/kill cells on the canvas
- (SPACE): save current state in a checkpoint and PLAY/PAUSE the simulation
- (w): REWIND to the state saved in the checkpoint
- (r): RESET to an empty canvas
- (R): populate the canvas with a random state
- (S): SAVE the current state to a file (`state.gol`) to load it in a future session

### Video demo:

https://github.com/user-attachments/assets/49c6b381-5d6d-41de-be87-2e272576a6e5
