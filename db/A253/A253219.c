/*
 * A253219  Number of n X 4 nonnegative integer arrays with upper left 0 and
 *          lower right its king-move distance away minus 2 and every value
 *          within 2 of its king move distance from the upper left and every
 *          value increasing by 0 or 1 with every step right, diagonally se or
 *          down.
 * https://oeis.org/A253219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) for n>9 Initial values:
 *            a(1)=3, a(2)=9, a(3)=19, a(4)=19, a(5)=268, a(6)=1249,
 *            a(7)=3140, a(8)=5986, a(9)=9792
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cached sequence values (1‑based indexing in cache[n‑1]) */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of values stored */
static size_t cache_cap = 0;  /* current capacity in seq_t elements */

/* first 9 values of the sequence */
static const seq_t init_vals[9] = {
    3, 9, 19, 19, 268, 1249, 3140, 5986, 9792
};

/* Ensure that cache has capacity for at least needed_len values. */
static void ensure_capacity(idx_t needed_len) {
    if (cache_cap >= (size_t)needed_len)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < (size_t)needed_len)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;                 /* offset is 1; 0 not used */

    /* Return initial values without caching when n ≤ 9. */
    if (n <= 9)
        return init_vals[n-1];

    /* Initialise cache with the first 9 values if not yet done. */
    if (cache_len < 9) {
        ensure_capacity(9);
        for (idx_t i = 0; i < 9; ++i)
            cache[i] = init_vals[i];
        cache_len = 9;
    }

    /* Extend cache up to the requested n, computing recurrence. */
    if (cache_len < n) {
        ensure_capacity(n);
        for (idx_t i = cache_len; i < n; ++i) {
            /* i is 0‑based for cache: cache[i] = a(i+1) */
            seq_t val = 3 * cache[i-1] - 3 * cache[i-2] + cache[i-3];
            cache[i] = val;
        }
        cache_len = n;
    }

    return cache[n-1];
}
