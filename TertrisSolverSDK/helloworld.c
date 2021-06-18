/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */


#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xtetrissolver.h"
#include "xil_cache.h"
#include "xtime_l.h"
#include "xuartps_hw.h"
#include <stdbool.h>

#define BOARDHEIGHT 15
#define BOARDWIDTH 6

u32 mainmemory[BOARDHEIGHT * BOARDWIDTH * 2 + 2];

void print_puzzle(u32 ram[], u32 start){
	printf("--------------------\n\r");
	for(u32 y = 0; y < BOARDHEIGHT; y++) {
		printf("|");
			for(u32 x = 0; x < BOARDWIDTH; x++) {
				if(ram[start + y*BOARDWIDTH+x] == 0){
					printf("   ");
				}else{
					printf(" %u ", (unsigned int)ram[start + y*BOARDWIDTH+x]);
				}

		    }
		printf("|\n");
		}
	printf("--------------------\n\r");
}

int getPuzzle(u32 seed, char *buf, int buf_size) {
    const char pieces[] = "IOTJLSZ";
    srand(seed);

    int puzzlesize = seed % 2 ? 4 : 12;
    if(buf_size < puzzlesize) return 0;

    for(int i = 0; i < puzzlesize; i++) {
        buf[i] = pieces[rand() % 7];
    }

    return 1;
}

void addChar(u32 *seed, u32 *seedSize, char number){

	if(*seedSize < 9){

		*seed = *seed * (unsigned int)(10) + (unsigned int)atoi(&number) ;

		XUartPs_SendByte(STDOUT_BASEADDRESS, number);
		*seedSize += 1;
	}

}

u32 getSeed(){
	bool wait = true;
	u32 seed = 0;
	u32 seedSize = 0;

	while(wait){
		if(XUartPs_IsReceiveData(STDIN_BASEADDRESS)) { //If the user has pressed a key
				char byte = XUartPs_RecvByte(STDIN_BASEADDRESS); //Read it in

				switch(byte){
					case '\n':
						wait = false;
						printf("\n\r");
					break;
					case '\r':
						wait = false;
						printf("\n\r");
					break;

					case '0':
						addChar(&seed, &seedSize, '0');
						break;
					case '1':
						addChar(&seed, &seedSize, '1');
						break;
					case '2':
						addChar(&seed, &seedSize, '2');
						break;
					case '3':
						addChar(&seed, &seedSize, '3');
						break;
					case '4':
						addChar(&seed, &seedSize, '4');
						break;
					case '5':
						addChar(&seed, &seedSize, '5');
						break;
					case '6':
						addChar(&seed, &seedSize, '6');
						break;
					case '7':
						addChar(&seed, &seedSize, '7');
						break;
					case '8':
						addChar(&seed, &seedSize, '8');
						break;
					case '9':
						addChar(&seed, &seedSize, '9');
						break;
					default:
						break;

				}



		}
	}
	return seed;


}

int main()
{
	while(1){

	init_platform();
	Xil_DCacheDisable();

	u32 seed = 0;

	//printf("\n\rEnter Seed : ");

	char word[15] = {'\n', '\r', 'E','n','t','e','r',' ','S','e','e','d',' ', ':',' '};

	for(u32 x = 0; x < 15;x++){
		XUartPs_SendByte(STDOUT_BASEADDRESS, word[x]); //Printf doesn't display before the lock for the user response
	}

	seed = getSeed();

	XTime startTime, endTime, executionTime;

	XTime_GetTime(&startTime);

	XTetrissolver hls;

	XTetrissolver_Initialize(&hls, XPAR_TETRISSOLVER_0_DEVICE_ID);
	XTetrissolver_Set_ram(&hls, (u32) mainmemory);

	printf("Seed : %u\r\n", (unsigned int)seed);
	printf("--------------------\n\r");
	printf("    TETRIS BOARD\n\r");

	u32 puzzleSize;

	if(seed % 2 == 0){

		puzzleSize = 12;

	}else{
		puzzleSize = 4;
	}

	XTetrissolver_Set_length_r(&hls, puzzleSize);

	char puzzle[12];

	getPuzzle(seed, puzzle, sizeof(puzzle));

	for(int x =0; x < 12; x++){
		XTetrissolver_Write_puzzle_Bytes(&hls,x, &puzzle[x], 12);
	}

	XTetrissolver_Start(&hls);
	while(!XTetrissolver_IsDone(&hls));

	print_puzzle(mainmemory, 0 );
	printf("ROTOTATION BOARD HEIGHT : %u\n\r", (unsigned int)mainmemory[BOARDWIDTH * BOARDHEIGHT * 2]);

	print_puzzle(mainmemory, BOARDWIDTH * BOARDHEIGHT );
	printf("NO ROTOTATION BOARD HEIGHT : %u\n\r", (unsigned int)mainmemory[BOARDWIDTH * BOARDHEIGHT * 2 + 1]);

	XTime_GetTime(&endTime);
	executionTime = endTime - startTime;
	float timeInSecs = 1.0 * executionTime / COUNTS_PER_SECOND;

	printf("Puzzle : ");
	printf("%.*s", (unsigned int)puzzleSize, puzzle);
	printf("\n\r");

 	printf("Time Taken : %f seconds\n\r ", timeInSecs );

	}
	cleanup_platform();

	return 0;
}
