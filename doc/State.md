# state_t
```c
/**
 * typedef struct state_t state_t;
 * Represents a morpx state
 */
typedef struct state_t {
	char board[81];
	char grid[9];
	char player;
	char status;
	char lastX;
	char lastY;
} state_t;
```

Represents a Morpx state.
All operations on it are unsafe if executed with illegal arguments, and are implemented as macros.

## Constants
These constants represent meaningful values for use with this module.
You **SHOULD** use the constant where possible, and **SHOULD NOT** use the numeric value, as they may change in the future.

### `STATE_UNREACHABLE` = `-1`
Represents neither the lack of value nor any player, so this either means a full cell or a draw.

### `STATE_NONE` = `0`
Represents the lack of a meaningful value, usually an empty cell or pending result.

### `STATE_P1` = `1`
Represents Player 1.

### `STATE_P2` = `2`
Represents Player 2.

### `STATE_NO_CHANGES` = `0x0`
Represents no major changes.

### `STATE_CELL_CHANGE` = `0x1`
The cell was taken.

### `STATE_STATUS_CHANGED` = `0x2`
The status of the game has changed. The game has ended.

## Internal functions
These functions are required for this module to work, but shouldn't be called from outside.
They are still made available in case they turn out to be useful, but they may break things if used without caution.

### `int state_getXYXY(int x1, int y1, int x2, int y2);`
Converts XYXY coordinates into an offset into a state.

### `int state_getXY(int x, int y);`
Converts XY coordinates into an offset into a state.

### `void state_setBoard(state_t* state, int x1, int y1, int x2, int y2, char value);`
Writes the state at the XYXY coordinates.

### ` void state_setGrid(state_t* state, int x, int y, char value);`
Writes the state at the XY coordinates.

## API functions
These functions are exposed to other modules.
None of them check for valid arguments, not even with `assert()`.
These functions are actually macros, so make sure their arguments are variables and aren't expensive to expand.

### `state_t state_alloc();`
Allocates a state struct.

### `void state_dealloc(state_t* state);`
Deallocates a state struct.

### `void state_init(state_t* state);`
Initializes an allocated state.

### `void state_copy(state_t* state, state_t* copy)`
Copies a state into another.

### `char state_getBoard(state_t* state, int x1, int y1, int x2, int y2);`
Reads the state at XYXY coordinates.

### `char state_getGrid(state_t* state, int x, int y);`
Reads the state at XY coordinates.

### `void state_play(state_t* state, int x1, int y1, int x2, int y2, int* cl);`
Plays at a the XYXY coordinate of a state.
Updates `cl` according to changes.

### `void state_copyPlay(state_t* state, state_t* copy, int x1, int y1, int x2, int y2, int* cl);`
Copies the state and plays in the copy.