#include "gf2d_sprite.h"
#include "gf2d_draw.h"

const Uint8 possibleVals[5] = {0, 0, 1, 2, 3};

typedef struct FlipGame {
	Uint16 score;
	Uint16 total;
	Uint8 size;
	Uint8 board[5][5];
	Uint8 rowSums[5], colSums[5];
	Uint8 rowBombs[5], colBombs[5];
} FlipGame;

FlipGame flipper;

Uint8 RollVal(int max) {
	return possibleVals[(int)(gfc_random() * (max + 2))];
}

int SetUpBoard() {
	for (int i = 0; i < 5; i++) {
		int max = 3;
		for (int j = 0; j < 5; j++) {
			max = MAX(MIN(3,MIN(10 - flipper.rowSums[i], 10 - flipper.colSums[j])), 0);
			Uint8 r = RollVal(max);
			flipper.board[i][j] = r;
			if (r > 0) {
				flipper.rowSums[i] += r;
				flipper.colSums[j] += r;
				flipper.total *= r;
			}
			else {
				flipper.rowBombs[i]++;
				flipper.colBombs[j]++;
			}
			printf("%d ", r);
		}
		printf("\n");
	}
	return flipper.total;
}