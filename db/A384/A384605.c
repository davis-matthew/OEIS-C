/*
 * A384605  Expansion of (1+x) / (1-x-4*x^2+2*x^3).
 * https://oeis.org/A384605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(0)=1, a(1)=2, a(2)=6; for n>=3 a(n) = a(n-1) + 4*a(n-2) -
 *            2*a(n-3) (recurrence from (1+x)/(1–x–4x²+2x³)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 52, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated capacity */
    static size_t len = 0;   /* number of computed entries (≤ cap) */

    /* Initialise cache on first call */
    if (cache == NULL) {
        /* start with first three terms */
        cap = 3;
        len = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* out of memory (unlikely) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
    }

    /* If the requested term is already computed, return it */
    if ((size_t)n < len) {
        return cache[n];
    }

    /* Ensure the cache is large enough to hold up to n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* out of memory */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms using the linear recurrence */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        cache[i] = cache[i-1]
                 + (seq_t)4 * cache[i-2]
                 - (seq_t)2 * cache[i-3];
    }
    len = n + 1;

    return cache[n];
}
