#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

const long long _100_TB_ARGUMENT = 100L << 40;

int main() {
	void *pointer_to_100tb = mmap(NULL, _100_TB_ARGUMENT, 
		                          PROT_READ, 
		                          MAP_NORESERVE | MAP_PRIVATE, 
		                          -1, 0);
	
	if (pointer_to_100tb == MAP_FAILED) {
   		printf("Error during allocating 100 TB!\n");
		return 1;
	}

	getchar();

	if (munmap(pointer_to_100tb, _100_TB_ARGUMENT) == -1) {
		printf("Error during deallocating 100 TB!\n");
		return 1;
	}

	return 0;
}