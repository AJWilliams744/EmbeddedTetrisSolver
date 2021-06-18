#include "tetrissolver.h"
//#define BOARDHEIGHT 10
//#define BOARDWIDTH 6

uint32 mainmemory[BOARDHEIGHT * BOARDWIDTH * 2 + 1];

int get_puzzle(uint32 seed, char *buf, int buf_size) {
    const char pieces[] = "IOTJLSZ";
    srand(seed);

    int puzzlesize = seed % 2 ? 4 : 12;
    if(buf_size < puzzlesize) return 0;

    for(int i = 0; i < puzzlesize; i++) {
        buf[i] = pieces[rand() % 7];
    }

    return 1;
}
void print_puzzle(uint32 ram[], uint32 start){
	for(uint32 y = 0; y < BOARDHEIGHT; y++) {
		printf("|");
			for(uint32 x = 0; x < BOARDWIDTH; x++) {
				if(ram[start + y*BOARDWIDTH+x] == 0){
					printf("   ");
				}else{
					printf(" %d ", ram[start + y*BOARDWIDTH+x]);
				}

		    }
		printf("|\n");
		}
}

int main() {

    char puzzle[12];

    //Seed 0 is a 'hard' puzzle, so size 12
    get_puzzle(1, puzzle, sizeof(puzzle));

    //Set up the slave inputs to the hardware
   // char arg1[] = "LOT";
    //char arg1[] = { "JLLJIIOOLSJO"};
    char arg1[] = { "LTTOOLST"};
    uint32 arg2 =  4;


    //Run the hardware
    tetrissolver(mainmemory, arg1, &arg2);

    print_puzzle(mainmemory, 0 );
    printf("ROT BOARD HEIGHT : %d\n", mainmemory[BOARDWIDTH * BOARDHEIGHT * 2]);
   // printf("\f");
    print_puzzle(mainmemory, BOARDWIDTH * BOARDHEIGHT );
    printf("NO ROT BOARD HEIGHT : %d", mainmemory[BOARDWIDTH * BOARDHEIGHT * 2 + 1]);


    //Read the slave outputs
    //printf("%.*s\n\r", 12, arg1);
    //printf("%d\n\r", arg2.to_int());


    return 0;
}


