#include "bitvec.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SLOT_BYTES sizeof(bitvec_slot_t)
#define SLOT_BITS (SLOT_BYTES * 8)
#define SLOT_MAX (~((bitvec_slot_t) 0))

#define CEILDIV(n, d) ((n) / (d) + ((n) % (d) == 0 ? 0 : 1))
#define BITCEIL(n) ((n) + SLOT_BITS - 1 - (SLOT_BITS - 1 + n) % SLOT_BITS)

#define SLOT(n) ((n) / SLOT_BITS)
#define SLOTCNT(n) CEILDIV(n, SLOT_BITS)
#define SLOT_BIT(n) (((bitvec_slot_t) 1) << n)

#define APPLY_MASK(x,s,m) ((s) ? ((x) | (m)) : ((x) & ~(m)))

#ifdef LIBBITVEC_CHECK_ENABLE
#define LIBBITVEC_CHECK(x) libbitvec_assert((x), __FILE__, __LINE__, #x)
#else
#define LIBBITVEC_CHECK(x)
#endif

static inline void
libbitvec_assert(int cond, const char *file, int line, const char *condstr)
{
	if (cond) return;

	fprintf(stderr, "libvec: Assertion failed at %s:%i (%s)\n",
		file, line, condstr);
	abort();
}

int
bitvec_init(struct bitvec *vec, size_t cap)
{
	LIBBITVEC_CHECK(vec != NULL);

	if (cap) {
		vec->data = calloc(SLOTCNT(cap), SLOT_BYTES);
		if (!vec->data) return errno;
	} else {
		vec->data = NULL;
	}

	vec->cap = BITCEIL(cap);

	return 0;
}

void
bitvec_deinit(struct bitvec *vec)
{
	LIBBITVEC_CHECK(vec != NULL);

	vec->cap = 0;
	free(vec->data);
}

int
bitvec_alloc(struct bitvec **bitvec, size_t cap)
{
	int rc;

	*bitvec = malloc(sizeof(struct bitvec));
	if (!*bitvec) return errno;

	rc = bitvec_init(*bitvec, cap);
	if (rc) {
		free(bitvec);
		return rc;
	}

	return 0;
}

void
bitvec_free(struct bitvec *vec)
{
	bitvec_deinit(vec);
	free(vec);
}

int
bitvec_reserve(struct bitvec *vec, size_t cnt)
{
	void *alloc;

	LIBBITVEC_CHECK(vec != NULL);

	cnt = BITCEIL(cnt);
	if (vec->cap >= cnt) return 0;

	alloc = realloc(vec->data, SLOTCNT(cnt) * SLOT_BYTES);
	if (!alloc) return errno;
	alloc = vec->data;
	memset(vec->data + SLOT(vec->cap), 0, SLOT(cnt) - SLOT(vec->cap));
	vec->cap = cnt;

	return 0;
}

int
bitvec_shrink(struct bitvec *vec, size_t cnt)
{
	void *alloc;

	LIBBITVEC_CHECK(vec != NULL);

	cnt = BITCEIL(cnt);
	if (vec->cap <= cnt) return 0;

	alloc = realloc(vec->data, SLOTCNT(cnt));
	if (!alloc) return errno;
	vec->data = alloc;
	vec->cap = cnt;

	return 0;
}

bool
bitvec_get(struct bitvec *vec, size_t pos)
{
	LIBBITVEC_CHECK(vec != NULL);

	if (pos >= vec->cap) return false;
	return !!(vec->data[pos / SLOT_BITS] & SLOT_BIT(pos % SLOT_BITS));
}

void
bitvec_set(struct bitvec *vec, size_t pos)
{
	LIBBITVEC_CHECK(vec != NULL && pos < vec->cap);

	vec->data[pos / SLOT_BITS] |= SLOT_BIT(pos % SLOT_BITS);
}

void
bitvec_clear(struct bitvec *vec, size_t pos)
{
	LIBBITVEC_CHECK(vec != NULL && pos < vec->cap);

	vec->data[pos / SLOT_BITS] &= ~SLOT_BIT(pos % SLOT_BITS);
}

void
bitvec_setn(struct bitvec *vec, size_t start, size_t end, bool set)
{
	bitvec_slot_t mask;
	size_t starti, endi, i;

	LIBBITVEC_CHECK(vec != NULL && end >= start && end <= vec->cap);

	if (start == end) return;

	starti = SLOT(start);
	end = SLOT(end - 1);

	if (starti == endi) {
		if (end % SLOT_BITS == 0)
			mask = SLOT_MAX - SLOT_BIT(start % SLOT_BITS);
		else
			mask = SLOT_BIT(end % SLOT_BITS) - SLOT_BIT(start % SLOT_BITS);
		vec->data[starti] = APPLY_MASK(vec->data[starti], set, mask);
	} else {
		mask = SLOT_MAX - SLOT_BIT(start % SLOT_BITS);
		vec->data[starti] = APPLY_MASK(vec->data[starti], set, mask);

		for (i = starti + 1; i <= endi - 1; i++)
			vec->data[i] = APPLY_MASK(vec->data[i], set, SLOT_MAX);

		mask = SLOT_BIT(end % SLOT_BITS) - SLOT_BIT(0);
		vec->data[endi] = APPLY_MASK(vec->data[endi], set, mask);
	}
}
