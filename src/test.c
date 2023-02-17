#include "bitvec.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int
main(int argc, const char **argv)
{
	struct bitvec bitvec;
	int i, *val;

	bitvec_init(&bitvec, 10);

	for (i = 1; i < argc; i++) {
		*val = atoi(argv[i]);
		if (bitvec_get(&bitvec, *val))
			printf("%i -> dup!\n", *val);
		bitvec_reserve(&bitvec, *val);
		bitvec_set(&bitvec, *val);
	}

	for (i = 0; i < 10; i++)
		printf("%i", bitvec_get(&bitvec, i));
	printf("\n");

	printf("bitvec len: %lu\n", bitvec.cap / (8 * sizeof(libbitvec_slot_t)));

	bitvec_deinit(&bitvec);
}
