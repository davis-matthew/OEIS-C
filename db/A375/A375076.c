/*
 * A375076  Numbers whose prime factorization exponents include at least one
 *          1, at least one 3 and no other exponents.
 * https://oeis.org/A375076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 4985, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A375076: Numbers whose prime factorisation exponents include
    at least one 1, at least one 3 and no other exponents.
    Sequence a(n) is generated in increasing order by testing every
    integer for the required exponent pattern.  The function is
    memoised so that calls can be made in any order.  This works
    for n up to ~10 000 in well under two seconds on typical
    64‑bit hardware. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_candidate.h"

/* Helper to grow a seq_t array to accommodate at least `need` elements. */
static void *grow_seq(void *ptr, size_t *cap, size_t need, size_t elem_size) {
    if (need > *cap) {
        size_t new_cap = (*cap == 0) ? 1 : *cap * 2;
        while (new_cap < need) new_cap <<= 1;
        ptr = realloc(ptr, new_cap * elem_size);
        *cap = new_cap;
    }
    return ptr;
}

/* Global state for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity */
static size_t cache_len = 0;   /* number of terms stored */
static uint64_t next_candidate = 2;  /* next integer to test */

/* Main function required by the build system */
seq_t a(idx_t n) {
    /* Grow cache to hold at least n terms */
    cache = (seq_t *)grow_seq(cache, &cache_cap, (size_t)n, sizeof(seq_t));

    while (cache_len < (size_t)n) {
        /* Test current candidate */
        if (is_candidate((uint64_t)next_candidate)) {
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cache[(size_t)(n - 1)];
}
