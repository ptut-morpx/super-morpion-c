#ifndef __STATE_H
#define __STATE_H

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

#define STATE_UNREACHABLE (-1)
#define STATE_NONE (0)
#define STATE_P1 (1)
#define STATE_P2 (2)

#define STATE_NO_CHANGES (0x0)
#define STATE_CELL_CHANGE (0x1)
#define STATE_STATUS_CHANGE (0x2)

/**
 * int state_getXYXY(int x1, int y1, int x2, int y2);
 * Converts XYXY coordinates into an offset into a state
 * @returns the offset
 */
#define state_getXYXY(x1, y1, x2, y2) ((x1)*27+(y1)*9+(x2)*3+(y2))

/**
 * int state_getXY(int x, int y);
 * Converts XY coordinates into an offset into a state
 * @returns the offset
 */
#define state_getXY(x, y) ((x)*3+(y))

/**
 * state_t* state_alloc();
 * Allocates a state struct
 * @returns the state struct, or NULL
 */
#define state_alloc() ((state_t*) malloc(sizeof(state_t)))

/**
 * void state_dealloc(state_t* state);
 * Deallocates a state struct
 */
#define state_dealloc(state) free(state)

/**
 * void state_init(state_t* state);
 * Initializes an allocated state
 */
#define state_init(state) do {\
	int i;\
	for(i=0; i<81; i++) (state)->board[i]=STATE_NONE;\
	for(i=0; i<9; i++) (state)->grid[i]=STATE_NONE;\
	(state)->lastX=-1;\
	(state)->lastY=-1;\
	(state)->status=STATE_NONE;\
} while(0)

/**
 * void state_copy(state_t* state, state_t* copy);
 * Copies a state into another
 */
#define state_copy(state, copy) do {\
	int i;\
	for(i=0; i<81; i++) (copy)->board[i]=(state)->board[i];\
	for(i=0; i<9; i++) (copy)->grid[i]=(state)->grid[i];\
	(copy)->lastX=(state)->lastX;\
	(copy)->lastY=(state)->lastY;\
	(copy)->status=(state)->status;\
} while(0)

/**
 * char state_getBoard(state_t* state, int x1, int y1, int x2, int y2);
 * Reads the state at XYXY coordinates
 * @returns the value at the XYXY coordinates
 */
#define state_getBoard(state, x1, y1, x2, y2) ((state)->board[state_getXYXY((x1), (y1), (x2), (y2))])

/**
 * void state_setBoard(state_t* state, int x1, int y1, int x2, int y2, char value);
 * Writes the state at the XYXY coordinates
 */
#define state_setBoard(state, x1, y1, x2, y2, value) ((state)->board[state_getXYXY((x1), (y1), (x2), (y2))]=value)

/**
 * char state_getGrid(state_t* state, int x, int y);
 * Reads the state at XY coordinates
 * @returns the value at the XYXY coordinates
 */
#define state_getGrid(state, x, y) ((state)->grid[state_getXY((x), (y))])

/**
 * void state_setGrid(state_t* state, int x, int y, char value);
 * Writes the state at the XY coordinates
 */
#define state_setGrid(state, x, y, value) ((state)->grid[state_getXY((x), (y))]=value)

/**
 * void state_play(state_t* state, int x1, int y1, int x2, int y2, int* cl);
 * Plays at a the XYXY coordinate of a state
 * Updates *cl according to changes
 */
#define state_play(state, x1, y1, x2, y2, cl) do {\
	int changes=STATE_NO_CHANGES;\
	int player=(state)->player;\
	\
	/* write board and last XY */\
	(state)->lastX=x2;\
	(state)->lastY=y2;\
	state_setBoard(state, x1, y1, x2, y2, (state)->player);\
	\
	/* check board */\
	if(\
		(\
			state_getBoard(state, x1, y1, x2, 0)==player&&\
			state_getBoard(state, x1, y1, x2, 1)==player&&\
			state_getBoard(state, x1, y1, x2, 1)==player\
		) || (\
			state_getBoard(state, x1, y1, 0, y2)==player&&\
			state_getBoard(state, x1, y1, 1, y2)==player&&\
			state_getBoard(state, x1, y1, 2, y2)==player\
		) || (\
			x2==y2 && (\
				state_getBoard(state, x1, y1, 0, 0)==player&&\
				state_getBoard(state, x1, y1, 1, 1)==player&&\
				state_getBoard(state, x1, y1, 2, 2)==player\
			)\
		) || (\
			x2==2-y2 && (\
				state_getBoard(state, x1, y1, 2, 0)==player&&\
				state_getBoard(state, x1, y1, 1, 1)==player&&\
				state_getBoard(state, x1, y1, 0, 2)==player\
			)\
		)\
	) {\
		state_setGrid(state, x1, y1, player);\
		changes|=STATE_CELL_CHANGE;\
		\
		/* check status */\
		if(\
			(\
				state_getGrid(state, x1, 0)==player&&\
				state_getGrid(state, x1, 1)==player&&\
				state_getGrid(state, x1, 2)==player\
			) || (\
				state_getGrid(state, 0, y1)==player&&\
				state_getGrid(state, 1, y1)==player&&\
				state_getGrid(state, 2, y1)==player\
			) || (\
				x1==y1 && (\
					state_getGrid(state, 0, 0)==player&&\
					state_getGrid(state, 1, 1)==player&&\
					state_getGrid(state, 2, 2)==player\
				)\
			) || (\
				x1==2-y1 && (\
					state_getGrid(state, 2, 0)==player&&\
					state_getGrid(state, 1, 1)==player&&\
					state_getGrid(state, 0, 2)==player\
				)\
			)\
		) {\
			(state)->status=player;\
			changes|=STATE_STATUS_CHANGE;\
		}\
	}\
	\
	/* check for full grid */\
	if(\
		!(changes&STATE_CELL_CHANGE)&&\
		(\
			state_getBoard(state, x1, y1, 0, 0)&&\
			state_getBoard(state, x1, y1, 0, 1)&&\
			state_getBoard(state, x1, y1, 0, 2)&&\
			state_getBoard(state, x1, y1, 1, 0)&&\
			state_getBoard(state, x1, y1, 1, 1)&&\
			state_getBoard(state, x1, y1, 1, 2)&&\
			state_getBoard(state, x1, y1, 2, 0)&&\
			state_getBoard(state, x1, y1, 2, 1)&&\
			state_getBoard(state, x1, y1, 2, 2)\
		)\
	) {\
		state_setGrid(state, x1, y1, STATE_UNREACHABLE);\
		changes|=STATE_CELL_CHANGE;\
	}\
	\
	/* check for full board */\
	if(\
		changes==STATE_CELL_CHANGE&&\
		(\
			state_getGrid(state, 0, 0)&&\
			state_getGrid(state, 0, 1)&&\
			state_getGrid(state, 0, 2)&&\
			state_getGrid(state, 1, 0)&&\
			state_getGrid(state, 1, 1)&&\
			state_getGrid(state, 1, 2)&&\
			state_getGrid(state, 2, 0)&&\
			state_getGrid(state, 2, 1)&&\
			state_getGrid(state, 2, 2)\
		)\
	) {\
		(state)->status=STATE_UNREACHABLE;\
		changes|=STATE_STATUS_CHANGE;\
	}\
	\
	/* end the turn */\
	(state)->player=((state)->player==STATE_P1?STATE_P2:STATE_P1);\
	\
	/* report changes */\
	if((cl)) *(cl)=changes;\
} while(0)

/**
 * void state_copyPlay(state_t* state, state_t* copy, int x1, int y1, int x2, int y2, int* cl);
 * Copies the state and plays in the copy
 */
#define state_copyPlay(state, copy, x1, y1, x2, y2, cl) (state_copy(state, copy), state_play(copy, x1, y1, x2, y2, cl))

#endif
