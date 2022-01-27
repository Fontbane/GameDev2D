#include "k_local.h"

u32 Random(int min, int max) {
	return (int)(gfc_random() * max)%min;
}