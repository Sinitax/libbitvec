#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef UINT64_MAX
typedef uint64_t bitvec_slot_t;
#else
typedef uint32_t bitvec_slot_t;
#endif

struct bitvec {
	size_t cap;
	bitvec_slot_t *data;
};

extern int libbitvec_errno;

int bitvec_init(struct bitvec *vec, size_t cap);
void bitvec_deinit(struct bitvec *vec);

struct bitvec *bitvec_alloc(size_t cap);
void bitvec_free(struct bitvec *vec);

int bitvec_reserve(struct bitvec *vec, size_t cnt);
int bitvec_shrink(struct bitvec *vec, size_t cnt);

bool bitvec_get(struct bitvec *vec, size_t pos);
void bitvec_set(struct bitvec *vec, size_t pos);
void bitvec_clear(struct bitvec *vec, size_t pos);
void bitvec_setn(struct bitvec *vec, size_t start, size_t end, bool set);
