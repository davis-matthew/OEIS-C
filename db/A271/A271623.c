/*
 * A271623  a(0)=7; a(n) = 7*a(n-1) + 1 if a(n-1) is odd, a(n) = a(n-1)/2
 *          otherwise.
 * https://oeis.org/A271623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 7; a(n) = 7*a(n-1) + 1 if a(n-1) is odd, otherwise a(n) =
 *            a(n-1)/2
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute terms of OEIS sequence A271623.
 * The function uses a static cache so that repeated calls
 * (in any order of indices) are O(1) after the term has been
 * computed once.  The cache is grown on demand to accommodate
 * larger indices. */
seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;  /* pointer to cached terms */
    static size_t cache_cap = 0;     /* capacity of allocated array */
    static size_t cache_len = 0;     /* number of terms already computed */

    /* Initialisation: term a(0) */
    if (cache_len == 0) {
        cache_cap = 1;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out of memory; caller must not rely on result */
        cache[0]   = (seq_t)7;
        cache_len  = 1;
    }

    /* Grow the cache if necessary to hold indices up to n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (cache_cap ? cache_cap << 1 : 1);
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out of memory */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        if ((prev & (seq_t)1) != (seq_t)0) {       /* prev is odd   */
            cache[i] = (seq_t)7 * prev + (seq_t)1;  /* 7*prev + 1 */
        } else {                                  /* prev is even  */
            cache[i] = prev / (seq_t)2;            /* prev / 2     */
        }
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
