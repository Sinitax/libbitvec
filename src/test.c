#include "bitvec.h"

#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LIBBITVEC_ERR(rc) errx(1, "libbitvec: %s", strerror(rc))

int
main(int argc, const char **argv)
{
	struct bitvec bitvec;
	int i, rc;
	int val;

	rc = bitvec_init(&bitvec, 10);
	if (rc) LIBBITVEC_ERR(rc);

	for (i = 1; i < argc; i++) {
		val = atoi(argv[i]);
		if (bitvec_get(&bitvec, val))
			printf("%i -> dup!\n", val);
		rc = bitvec_reserve(&bitvec, val);
		if (rc) LIBBITVEC_ERR(rc);
		bitvec_set(&bitvec, val);
	}

	for (i = 0; i < 10; i++)
		printf("%i", bitvec_get(&bitvec, i));
	printf("\n");

	printf("bitvec len: %lu\n", bitvec.cap / (8 * sizeof(bitvec_slot_t)));

	bitvec_deinit(&bitvec);
}
