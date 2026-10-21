/*
 * A264570  Number of (2+1) X (n+1) arrays of permutations of 0..n*3+2 with
 *          each element having directed index change 1,0 1,1 0,-1 or -1,1.
 * https://oeis.org/A264570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 8*a(n-4) with a(1)=1, a(2)=2, a(3)=4, a(4)=8
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 49, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache: cache[0] holds a(1), cache[1] holds a(2), etc. */
static seq_t *cache = NULL;
static idx_t cache_size = 0; /* number of terms currently stored */

/* Compute the n-th term of OEIS A264570; n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for underflow, not used */

    /* Return cached value if already computed. */
    if (n <= cache_size) {
        return cache[n - 1];
    }

    /* Grow the cache to hold terms up to n. */
    seq_t *new_cache;
    if (cache == NULL) {
        new_cache = (seq_t *)malloc(n * sizeof(seq_t));
        if (!new_cache) abort();   /* allocation failed */
    } else {
        new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache) abort();   /* allocation failed */
    }
    cache = new_cache;

    /* Initialize the first four terms if they are missing. */
    if (cache_size < 1 && n >= 1) cache[0] = (seq_t)1;
    if (cache_size < 2 && n >= 2) cache[1] = (seq_t)2;
    if (cache_size < 3 && n >= 3) cache[2] = (seq_t)4;
    if (cache_size < 4 && n >= 4) cache[3] = (seq_t)8;

    /* Compute new terms using the recurrence. */
    idx_t start = cache_size + 1;
    if (start < 5) start = 5;   /* recurrence is defined for n ≥ 5 */
    for (idx_t i = start; i <= n; ++i) {
        /* cache[i-1] = 2*cache[i-2] + 8*cache[i-5] */
        cache[i - 1] = (seq_t)2 * cache[i - 2] + (seq_t)8 * cache[i - 5];
    }

    cache_size = n;
    return cache[n - 1];
}
