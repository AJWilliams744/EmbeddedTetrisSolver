#include "tetrissolver.h"

const int1 pieces[7][4][4][4] = {
    { //I
        {
            {1, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        }, {
            {1, 0, 0, 0},
            {1, 0, 0, 0},
            {1, 0, 0, 0},
            {1, 0, 0, 0}
        }
    },
    { //O
        {
            {1, 1, 0, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        }
    },
    { //T
        {
            {1, 1, 1, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        }, {
            {1, 0, 0, 0},
            {1, 1, 0, 0},
            {1, 0, 0, 0},
            {0, 0, 0, 0}
        }, {
            {0, 1, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        }, {
            {0, 1, 0, 0},
            {1, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0}
        }
    },
    { //J
        {
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0}
        }, {
            {1, 0, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        }, {
            {1, 1, 0, 0},
            {1, 0, 0, 0},
            {1, 0, 0, 0},
            {0, 0, 0, 0}
        }, {
            {1, 1, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        }
    },
    { //L
        {
            {1, 0, 0, 0},
            {1, 0, 0, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0}
        }, {
            {1, 1, 1, 0},
            {1, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        }, {
            {1, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0}
        },  {
            {0, 0, 1, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        }
    },
    { //S
        {
            {0, 1, 1, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        }, {
            {1, 0, 0, 0},
            {1, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0}
        }
    },
    { //Z
        {
            {1, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        }, {
            {0, 1, 0, 0},
            {1, 1, 0, 0},
            {1, 0, 0, 0},
			{0, 0, 0, 0}
        }
    }
};

int32 tetrissolver(uint32 *ram, char puzzle[12], uint32 *length) {
#pragma HLS ARRAY_PARTITION variable=pieces complete dim=2
	#pragma HLS INTERFACE m_axi port=ram offset=slave bundle=MAXI
	#pragma HLS INTERFACE s_axilite port=puzzle bundle=AXILiteS register
	#pragma HLS INTERFACE s_axilite port=length bundle=AXILiteS register
	#pragma HLS INTERFACE s_axilite port=return bundle=AXILiteS register

	int4 currentTallBestHeight = BOARDHEIGHT;
	int4 currentShortBestHeight = BOARDHEIGHT;

	int1 bestBoardNo[BOARDHEIGHT][BOARDWIDTH] = { // Initialising makes sure all values are zero on start
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
	};

	int1 bestBoardRot[BOARDHEIGHT][BOARDWIDTH] = {
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
		};

	if(*length > int4(6)){ //Solve max 6 rows at a time
		int4 middle = int4(6);


		solve(ram, puzzle, middle, false, &currentShortBestHeight, &bestBoardNo,int4(0), middle);
		solve(ram, puzzle, middle, true, &currentShortBestHeight, &bestBoardRot, int4(0), middle);


		solve(ram, puzzle, *length, false, &currentTallBestHeight, &bestBoardNo,int4(6), *length);
		solve(ram, puzzle, *length, true, &currentTallBestHeight, &bestBoardRot,int4(6), *length);

	}else{
		solve(ram, puzzle, *length, false, &currentTallBestHeight, &bestBoardNo, int4(0), *length);
		solve(ram, puzzle, *length, true, &currentTallBestHeight, &bestBoardRot, int4(0), *length);
	}

	saveBoard(bestBoardRot, ram,0);
	saveBoard(bestBoardNo, ram,BOARDHEIGHT * BOARDWIDTH);

	return 0;
}

void solve(uint32 * ram, char puzzle[12], uint32 length, bool shouldSwap, int4 *currentBestHeight, int1 (*bestBoard)[BOARDHEIGHT][BOARDWIDTH], int4 startLoop, int4 endLoop){


	int4 rowCounter[12] = {0,0,0,0,0,0,0,0,0,0,0,0}; // Data for "Recursion Stack"
	int4 rotation[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
	int4 rotationSize[12] = {0,0,0,0,0,0,0,0,0,0,0,0};

	int1 middleBaseboard[BOARDHEIGHT][BOARDWIDTH];

	int1 baseBoard[BOARDHEIGHT + 3][BOARDWIDTH + 3] = { // Initialise inside loop to remove need for reseting added 3 for unrolling of checker
				{0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0},
	};
#pragma HLS ARRAY_PARTITION variable=baseBoard complete dim=2

	int4 bestHeight = BOARDHEIGHT;
	int4 boardHeight; // Initialise outside of loop for increased speed

	int4 arrayEnd = length - 1;

	bool end = false;
	if(startLoop == int4(6)){
		copyBoard(*bestBoard, &middleBaseboard);
	}

	complet : while(!end){ // Keep looping until all paths have been checked
#pragma HLS LOOP_TRIPCOUNT min=5 max=10

		if(startLoop == int4(6)){
			//copyBoard(*bestBoard, &middleBaseboard);
			setBoard(&baseBoard,middleBaseboard);
		}else{
			initializeBoard(&baseBoard);
		}


		puzzle : for(int4 x = startLoop; x < endLoop; x++){ //Loop through all pieces in the puzzle up to a max of 6
#pragma HLS LOOP_TRIPCOUNT min=4 max=6
				if(rowCounter[x] >= int4(6)){ // If all paths with the current row have been checked

					if(shouldSwap){// Handles different arrays depending on rotations or not
						if(rotation[x] >= rotationSize[x] - 1){
							if(x == startLoop){ //If all paths have been checked end the main loop
								end = true;
							}else{

								rowCounter[x-1] += 1; // Increment the position of the previous piece
								rowCounter[x] = 0; // Reset current piece position

								rotation[x] = 0; // Reset the rotation location
							}
						}else{
							rowCounter[x] = 0;
							rotation[x] += 1;
						}


					}else{
						if(x == startLoop){ //If all paths have been checked end the main loop
						end = true;
						}

						rowCounter[x-1] += 1;
						rowCounter[x] = 0;

					}

					x = 0;
					goto finishedRow; // Quits inside loop to reset the for loop
				}

				int4 result[6][2]  = {
						{0, 0},
						{0, 0},
						{0, 0},
						{0, 0},
						{0, 0},
						{0, 0}};

				int4 pieceNumber = int4(0); // Store the number for accessing the pieces array
				int4 width = int4(0); // Width of the piece
				int4 height = int4(0); // Height of the piece
				int4 rotationCount= int4(0); // How many rotations are allowed for this piece

				getPieceData(puzzle[x], &pieceNumber, &width, &height, &rotationCount);

				rotationSize[x] = rotationCount; // Set the maximum rotations a piece can have

				if(shouldSwap){
					swapRotationData(&width, &height, rotation[x]); //Swaps the width and height rotation[x] amount of times

				}

				findValidPositionComplete(baseBoard, pieces[pieceNumber][rotation[x]], width, height, &result);

				if(result[rowCounter[x]][1] == int4(0)){ // If the current result is empty

					zeroCheckLoop:for(int4 zeroCheck = rowCounter[x]; zeroCheck < int4(6); zeroCheck++){// Keep looping till found result
					#pragma HLS LOOP_TRIPCOUNT min=1 max=5


						rowCounter[x] += 1;
						if(result[zeroCheck][1] == int4(1)){

							goto endLoop; // Break from loop when found

						}
					}
					x -= 1;
					goto endPuzzleRow; // If nothing found rerun the cycle
					endLoop:;
				}


				// If the location of the new piece is lower than the global best (only rotation solver uses this) or the local best move on
				if(result[rowCounter[x]][0] < BOARDHEIGHT - bestHeight or result[rowCounter[x]][0] < BOARDHEIGHT - *currentBestHeight){
					rowCounter[x] += 1;
					x -= 1;

				}else{
					placePiece(&baseBoard, pieces[pieceNumber][rotation[x]], rowCounter[x], result[rowCounter[x]][0]); // Place the piece at the location
				}



				endPuzzleRow:; // Used for skipping the placing of a piece without needing a boolean

			}

			rowCounter[arrayEnd] += 1; // Change the final puzzle piece location


			boardHeight = countHeight(baseBoard);


			if(boardHeight < bestHeight ){ // only store if the current board is better than the local best

				if(!shouldSwap){
					*currentBestHeight = boardHeight; // Used to increase speed of rotation solver
				}

				storeBoard(baseBoard, bestBoard); // store the board in a global variable

				bestHeight = boardHeight;

			}



			finishedRow:; // Used for skipping most computation if cycle skip is needed

	}

	if(shouldSwap){
		ram[(BOARDHEIGHT*BOARDWIDTH * 2) ] = bestHeight.to_int(); // Store the best height into ram
	}else{
		ram[(BOARDHEIGHT*BOARDWIDTH * 2) + 1 ] = bestHeight.to_int();
	}


}

void initializeBoard(int1 (*board)[BOARDHEIGHT + 3][BOARDWIDTH + 3]){

	for(int4 y = 0; y < BOARDHEIGHT; y++) {
					for(int4 x = 0; x < BOARDWIDTH; x++) {
#pragma HLS UNROLL
						(*board)[y][x] = int1(0);
					}

	}
}

void setBoard(int1 (*board)[BOARDHEIGHT + 3][BOARDWIDTH + 3], int1 copyBoard[BOARDHEIGHT][BOARDWIDTH]){
	for(int4 y = 0; y < BOARDHEIGHT; y++) {
					for(int4 x = 0; x < BOARDWIDTH; x++) {
#pragma HLS UNROLL

							(*board)[y][x] = copyBoard[y][x];

					}

	}
}

//stores base board into another board, board copy is usually a global variable
void storeBoard(int1 baseBoard[BOARDHEIGHT + 3][BOARDWIDTH + 3], int1 (*boardCopy)[BOARDHEIGHT][BOARDWIDTH]){
	storeOutside : for(int4 y = 0; y < BOARDHEIGHT; y++){
#pragma HLS UNROLL
		storeInside : for(int4 x = 0; x < BOARDWIDTH; x++){
#pragma HLS UNROLL
			(*boardCopy)[y][x] = baseBoard[y][x];
		}
	}

}

//copy's board into another board, board copy is usually a global variable (used for initialisation)
void copyBoard(int1 baseBoard[BOARDHEIGHT][BOARDWIDTH], int1 (*boardCopy)[BOARDHEIGHT][BOARDWIDTH]){
	storeOutside : for(int4 y = 0; y < BOARDHEIGHT; y++){
#pragma HLS UNROLL
		storeInside : for(int4 x = 0; x < BOARDWIDTH; x++){
#pragma HLS UNROLL
			(*boardCopy)[y][x] = baseBoard[y][x];
		}
	}

}

// Keeps swapping the width and height rotationCount worth of times
void swapRotationData(int4 *width, int4 *height, int4 rotationCount){
	swap : for(int4 x = 0; x < rotationCount; x++){
#pragma HLS LOOP_TRIPCOUNT min=1 max=4
		int4 temp;
		temp = *width;
		*width = *height;
		*height = temp;
	}


}

//Gets all of the data associated with a piece just from its char
void getPieceData(char pieceName, int4 *pieceNumber , int4 *width, int4 *height, int4 *rotationCount){
	switch(pieceName){

	    case 'I':
	    	*pieceNumber = int4(0);
			*width = int4(4);
	    	*height = int4(1);
	    	*rotationCount = int4(2);
	      break;

	    case 'O':
	    	*pieceNumber = int4(1);
			*width = int4(2);
			*height = int4(2);
			*rotationCount = int4(1);
	      break;

	    case 'T':
	    	*pieceNumber = int4(2);
			*width = int4(3);
			*height = int4(2);
			*rotationCount = int4(4);
		  break;

	    case 'J':
	    	*pieceNumber = int4(3);
			*width = int4(2);
			*height = int4(3);
			*rotationCount = int4(4);
		  break;

	    case 'L':
	    	*pieceNumber = int4(4);
			*width = int4(2);
			*height = int4(3);
			*rotationCount = int4(4);
		  break;

	    case 'S':
	    	*pieceNumber = int4(5);
			*width = int4(3);
			*height = int4(2);
			*rotationCount = int4(2);
		  break;

	    case 'Z':
	    	*pieceNumber = int4(6);
			*width = int4(3);
			*height = int4(2);
			*rotationCount = int4(2);
		  break;

	    default:
	    	*pieceNumber = int4(0);
			*width = int4(4);
			*height = int4(1);
			*rotationCount = int4(2);
	}
}

//Places a piece onto the board
void placePiece(int1 (*board)[BOARDHEIGHT + 3][BOARDWIDTH + 3], const int1 piece[4][4],  int4 xLoc, int4 yLoc){
	placeOutside : for(int4 y = 0; y < 4; y++) {
#pragma HLS UNROLL
		    placeInside : for(int4 x = 0; x < 4; x++) {
#pragma HLS UNROLL
		    	// Only change where piece is 1
		    	if(piece[y][x] == 1){
		    		(*board)[y + yLoc][x + xLoc] = int1(1);
		    	}
		    }
	}


}

//Counts the height of the board, checks all of row x and if a 1 is found returns the row count from the top down
int4 countHeight(int1 board[BOARDHEIGHT + 3][BOARDWIDTH + 3]){

	bool found;

heightOutside: for (int4 y = 0; y < BOARDHEIGHT; y++)
	{
		found = false;
		heightInside: for (int4 x = 0; x < BOARDWIDTH; x++)
		{
		#pragma HLS UNROLL
				if (board[y][x] == int1(1))
				{
					found = true;
				}
		}


		if(found){
			return BOARDHEIGHT - y;
		}
	}

	return BOARDHEIGHT;

}

//Saves the board in ram, slow so only called twice at the end
void saveBoard(int1 board[BOARDHEIGHT][BOARDWIDTH],uint32 *ram , int10 start){

	for(int4 y = 0; y < BOARDHEIGHT; y++) {
		for(int4 x = 0; x < BOARDWIDTH; x++) {

	        ram[start + y*BOARDWIDTH+x] = board[y][x];
	    }
	}
}

// Checks the square to see if its a valid position
bool checkSquare(int1 board[BOARDHEIGHT + 3][BOARDWIDTH + 3],const int1 piece[4][4],int4 startX, int4 startY){
	bool found = true;

	squareOutside : for(int4 y = 0; y < 4; y++){
#pragma HLS UNROLL
			squareInside : for( int4 x = 0; x < 4; x++){
#pragma HLS UNROLL

				if(piece[y][x] == int1(1) and board[y+startY][x + startX] == int1(1)){
					found = false;

				}

			}
	}

	return found;
}

//Checks above the piece to make sure there are no pieces above it
bool checkAbove(int1 board[BOARDHEIGHT + 3][BOARDWIDTH + 3], int4 startX, int4 endX, int4 startY){
	bool found = true;

	aboveOutside: for(int4 x = 0; x < 4; x ++){
#pragma HLS UNROLL
		aboveInside : for(int4 y = 0; y < BOARDHEIGHT; y++){
#pragma HLS UNROLL

			if(y < startY and x < endX-startX ){ // Only checks the pieces width

				if(board[y][x + startX] == int1(1)){
					found = false;
				}
			}

		}
	}



	return found;
}

// Checks below the piece to check there is a block below
bool checkBelow(int1 board[BOARDHEIGHT + 3][BOARDWIDTH + 3], const int1 piece[4][4], int4 startX, int4 startY, int4 endY){

	if(endY == BOARDHEIGHT){ // If bottom piece
		return true;
	}

	bool found = false;

	belowOutside : for(int4 y = 0; y < 4; y ++){
#pragma HLS UNROLL
		belowInside : for(int4 x = 0; x < 4; x ++){
#pragma HLS UNROLL

			// Only checks where piece is

			if(piece[y][x] == int1(1) and board[startY + y + int4(1)][x + startX] == int1(1)){
				found = true;

			}

		}

	}

	return found;
}

// Checks below the piece to check there is a block below
bool checkImidateAbove(int1 board[BOARDHEIGHT + 3][BOARDWIDTH + 3], const int1 piece[4][4], int4 startX, int4 startY){

	bool found = true;

	belowOutside : for(int4 y = 0; y < 4; y ++){
#pragma HLS UNROLL
		belowInside : for(int4 x = 0; x < 4; x ++){
#pragma HLS UNROLL

			// Only checks where piece is

			if(piece[y][x] == int1(1) and board[startY + y - int4(1)][x + startX] == int1(1)){
				found = false;

			}

		}

	}

	return found;
}

//Find all valid positions for a piece and its current rotation
void findValidPositionComplete(int1 board[BOARDHEIGHT + 3][BOARDWIDTH + 3], const int1 piece[4][4], int4 width, int4 height, int4 (*result)[6][2]){
#pragma HLS DEPENDENCE variable=result inter false
#pragma HLS INLINE
	findInsde :for(int4 x = 0; x < BOARDWIDTH; x++){
		findOutside : for(int4 y = BOARDHEIGHT - 1; y >= 1 ; y--){
#pragma HLS UNROLL

			if(x <= BOARDWIDTH - width and y <= BOARDHEIGHT - height){

				// If piece fits, if piece place follows all the rules of tetris
				if(checkSquare(board, piece, x, y) and checkBelow(board, piece, x, y, y + height) and checkImidateAbove(board, piece, x,y) and checkAbove(board, x, x +width, y - 1) ){

					(*result)[x][0] = y; // Stores a valid position
					(*result)[x][1] = int4(1); //Each x value can only have the possibility of one valid position


				}
			}

		}

	}
}
