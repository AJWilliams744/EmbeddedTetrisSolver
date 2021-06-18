#ifndef __TETRISSOLVER_H_
#define __TETRISSOLVER_H_

#define BOARDHEIGHT 15
#define BOARDWIDTH 6

#include <stdio.h>
#include <stdlib.h>
#include <ap_int.h>

//Typedefs
typedef unsigned int uint32;
typedef int int32;
typedef ap_uint<1> int1;
typedef ap_uint<2> int2;
typedef ap_uint<4> int4;
typedef ap_uint<10> int10;

int32 tetrissolver(uint32 *ram, char puzzle[12], uint32 *length);

void solve(uint32 * ram, char puzzle[12], uint32 length, bool shouldSwap, int4 *currentBestHeight, int1 (*bestBoard)[BOARDHEIGHT][BOARDWIDTH], int4 startLoop, int4 endLoop);

void findValidPositionComplete(int1 board[BOARDHEIGHT + 3][BOARDWIDTH + 3], const int1 piece[4][4], int4 width, int4 height, int4 (*result)[6][2]);

bool checkSquare(int1 board[BOARDHEIGHT + 3][BOARDWIDTH + 3],const int1 piece[4][4],int4 startX, int4 startY);
bool checkBelow(int1 board[BOARDHEIGHT + 3][BOARDWIDTH + 3], int1 piece[4][4], int4 startX, int4 startY, int4 endY);
bool checkImidateAbove(int1 board[BOARDHEIGHT + 3][BOARDWIDTH + 3], const int1 piece[4][4], int4 startX, int4 startY);
bool checkAbove(int1 board[BOARDHEIGHT + 3][BOARDWIDTH + 3], int4 startX, int4 endX, int4 startY);

void storeBoard(int1 baseBoard[BOARDHEIGHT + 3][BOARDWIDTH + 3], int1 (*boardCopy)[BOARDHEIGHT][BOARDWIDTH]);
void copyBoard(int1 baseBoard[BOARDHEIGHT][BOARDWIDTH], int1 (*boardCopy)[BOARDHEIGHT][BOARDWIDTH]);
void saveBoard(int1 board[BOARDHEIGHT][BOARDWIDTH],uint32 *ram, int10 start);

void placePiece(int1 (*board)[BOARDHEIGHT + 3][BOARDWIDTH + 3], const int1 piece[4][4],  int4 xLoc, int4 yLoc);
void getPieceData(char pieceName, int4 *pieceNumber , int4 *width, int4 *height, int4 *rotationCount);

int4 countHeight(int1 board[BOARDHEIGHT + 3][BOARDWIDTH + 3]);

void swapRotationData(int4 *width, int4 *height, int4 rotationCount);

void initializeBoard(int1 (*board)[BOARDHEIGHT + 3][BOARDWIDTH + 3]);
void setBoard(int1 (*board)[BOARDHEIGHT + 3][BOARDWIDTH + 3], int1 copyBoard[BOARDHEIGHT][BOARDWIDTH]);


#endif
