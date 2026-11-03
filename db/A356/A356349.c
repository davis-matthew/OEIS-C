/*
 * A356349  Primitive Niven numbers: terms of A005349 that are not ten times
 *          another term of A005349.
 * https://oeis.org/A356349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primitive Niven numbers: a(n) is the n‑th number that is divisible
   by the sum of its decimal digits and is not a multiple of 10 whose
   quotient is also a Niven number. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static bool is_niven(seq_t n) {
    seq_t s = digit_sum(n);
    uint64_t sv = (uint64_t)s;          /* sum of digits fits in 64 bits */
    return (n % sv == 0);
}

static seq_t *seq_cache = NULL;   /* array of found primitive Niven numbers */
static size_t seq_cap = 0;        /* current capacity of seq_cache          */
static size_t seq_len = 0;        /* number of elements stored              */
static seq_t next_candidate = 1;  /* next integer to test                  */
static int seq_initialized = 0;

static void init_seq_cache(void) {
    if (seq_initialized) return;
    seq_cap = 16;
    seq_cache = seq_alloc(seq_cap);
    seq_len = 0;
    next_candidate = 1;
    seq_initialized = 1;
}

seq_t a(idx_t n) {
    init_seq_cache();

    if (n == 0) return 0;                /* offset starts at 1 */

    while (seq_len < n) {
        seq_t num = next_candidate;

        if (is_niven(num)) {
            if (!((num % (seq_t)10) == 0 && is_niven(num / (seq_t)10))) {
                if (seq_len == seq_cap) {    /* grow the cache */
                    size_t new_cap = seq_cap * 2;
                    seq_t *tmp = seq_realloc(seq_cache, seq_cap, new_cap);
                    if (!tmp) return 0;      /* allocation failed */
                    seq_cache = tmp;
                    seq_cap = new_cap;
                }
                seq_cache[seq_len++] = num;
            }
        }
        next_candidate++;
    }

    return seq_cache[n - 1];
}
