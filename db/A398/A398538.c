/*
 * A398538  Number of n-tuples (a_1, ..., a_n) in (Z/3Z)^n such that
 *          M(a_1)*M(a_2)*...*M(a_n) = -I, where M(a) = [a, -1; 1, 0].
 * https://oeis.org/A398538
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A398538
   a(2)=1, a(3)=1, a(4)=2, a(5)=10
   a(n) = 2*a(n-1) + 6*a(n-3) + 9*a(n-4)   for n ≥ 6
   Memoised, O(n) time and memory, works for all seq_t/idx_t pairs. */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache of computed terms          */
static idx_t  max_computed = 1;      /* highest index stored so far      */
static size_t cache_cap   = 0;       /* allocated capacity in elements    */

/* Ensure the cache can hold indices up to idx (inclusive).  Resize by doubling. */
static void grow_cache(idx_t idx) {
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap <= (size_t)idx) new_cap <<= 1;          /* double until large enough */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                                   /* allocation failure */
    cache     = tmp;
    cache_cap = new_cap;
}

/* Return a(n) for n ≥ 2.  For n < 2 the value is undefined; we return 0. */
seq_t a(idx_t n) {
    if (n < 2) return 0;

    /* Initialise cache on first call. */
    if (!cache) {
        cache_cap   = 8;                     /* start with small capacity */
        cache       = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) abort();                 /* allocation failure */
        max_computed = 1;                    /* nothing beyond index 1 yet */
    }

    /* Grow cache to accommodate index n. */
    if ((size_t)(n) >= cache_cap) grow_cache(n);

    /* Compute any missing terms up to n. */
    while (max_computed < n) {
        idx_t i = max_computed + 1;
        if (i == 2)          cache[i] = 1;
        else if (i == 3)     cache[i] = 1;
        else if (i == 4)     cache[i] = 2;
        else if (i == 5)     cache[i] = 10;
        else                 cache[i] = 2 * cache[i-1] + 6 * cache[i-3] + 9 * cache[i-4];
        ++max_computed;
    }

    return cache[n];
}
