/*
 * A328782  Integers k such that k and k^2 contain the same number > 0 of
 *          digits zero in their decimal expansion.
 * https://oeis.org/A328782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 7108, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A328782: 
 * a(n) = the nth integer k (with a(1)=0) such that k and k^2 contain the
 * same positive number of zero digits in their decimal expansion.
 * The function builds all terms on demand, storing them in a static
 * dynamic array. */

/* include utilities for digit counting */
#include "../utils/digit_counts.h"
#include <stdlib.h>
#include <stdbool.h>

/* static cache of computed terms */
static seq_t *cache = NULL;   /* array of terms */
static size_t cache_size  = 0;/* number of stored terms */
static size_t cache_cap   = 0;/* capacity of array */

/* last integer examined (k) */
static idx_t next_k = 0;

/* helper to grow the cache array */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        /* In a real build, handle out‑of‑memory gracefully; here we abort */
        abort();
    }
    cache = new_ptr;
    cache_cap = new_cap;
}

/* main function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1, behaviour below is irrelevant */

    /* extend cache until we have at least n terms */
    while (cache_size < (size_t)n) {
        seq_t k = (seq_t)next_k;

        /* count zeros in k */
        unsigned char cnt_k[10];
        digit_counts(k, cnt_k);
        if (cnt_k[0] == 0) { /* need >0 zeros */
            ++next_k;
            continue;
        }

        /* count zeros in k^2 */
        seq_t k2 = k * k;
        unsigned char cnt_k2[10];
        digit_counts(k2, cnt_k2);
        if (cnt_k2[0] == cnt_k[0]) {
            /* store this term */
            if (cache_size >= cache_cap) grow_cache();
            cache[cache_size] = k;
            ++cache_size;
        }
        ++next_k;
    }

    return cache[n - 1];
}
