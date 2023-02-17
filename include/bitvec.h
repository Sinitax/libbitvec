#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef LIBBITVEC_ASSERT_ENABLE

#include <stdio.h>

#define LIBBITVEC_ASSERT(x) libbitvec_assert((x), __FILE__, __LINE__, #x)

static inline void libbitvec_assert(int cond,
	const char *file, int line, const char *condstr)
{
	if (cond) return;

	fprintf(stderr, "libvec: Assertion failed at %s:%i (%s)\n",
		file, line, condstr);
	abort();
}

#else
#define LIBBITVEC_ASSERT(x)
#endif

#ifdef LIBBITVEC_HANDLE_ERRS

#include <errno.h>
#include <stdio.h>

#define LIBBITVEC_HANDLE_ERR(x) libbitvec_err(__FILE__, __LINE__, x)

static inline void libbitvec_err(const char *file, int line, const char *info)
{
	fprintf(stderr, "libvec: %s at %s:%i: %s\n",
		info, file, line, strerror(errno));
	exit(1);
}

#else
#define LIBBITVEC_HANDLE_ERR(x)
#endif

#ifdef UINT64_MAX
typedef uint64_t libbitvec_slot_t;
#else
typedef uint32_t libbitvec_slot_t;
#endif

struct bitvec {
	size_t cap;
	libbitvec_slot_t *data;
};

bool bitvec_init(struct bitvec *vec, size_t cap);
void bitvec_deinit(struct bitvec *vec);

struct bitvec *bitvec_alloc(size_t cap);
void bitvec_free(struct bitvec *vec);

bool bitvec_reserve(struct bitvec *vec, size_t cnt);
bool bitvec_shrink(struct bitvec *vec, size_t cnt);

bool bitvec_get(struct bitvec *vec, size_t pos);
void bitvec_set(struct bitvec *vec, size_t pos);
void bitvec_clear(struct bitvec *vec, size_t pos);
void bitvec_setn(struct bitvec *vec, size_t start, size_t end, bool set);
