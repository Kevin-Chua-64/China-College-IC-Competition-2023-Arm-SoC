#include <stdio.h>
#include "CortexM3.h"
#include "run.h"

int main(void) {
//	printf("******************\n");
//	printf("cortex-m3 startup!\n");
//	printf("******************\n");
	
	all_init();
	
	while(1){
		run_game();
	}
}
