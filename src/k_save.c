#include <stdio.h>

#include "k_save.h"

SaveBlock1 LoadSave(char* filename) {
	FILE* f= fopen(filename, 'R');
	fread(save1.map, sizeof(Map), 1, f);
}