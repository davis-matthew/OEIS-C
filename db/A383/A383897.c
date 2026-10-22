/*
 * A383897  Expansion of e.g.f. log(1 + x)/(1 - 2*x).
 * https://oeis.org/A383897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*a(n-1) + 2*(n-1)^2*a(n-2); a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A383897 with memoisation for arbitrary order calls. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;     /* array of computed terms */
    static size_t cache_cap = 0;    /* current capacity of cache */
    static size_t cache_len = 0;    /* number of terms already computed */

    /* Ensure enough space for index 'n' */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 2;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                    /* allocation failed – return 0 */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base values if not yet done */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;          /* a(0) */
        cache_len = 1;
    }
    if (cache_len == 1 && (size_t)n >= 1) {
        cache[1] = (seq_t)1;          /* a(1) */
        cache_len = 2;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* Compute a(i) = (i+1)*a(i-1) + 2*(i-1)^2*a(i-2) */
        seq_t term1 = (seq_t)(i + 1) * cache[i - 1];
        seq_t term2 = (seq_t)(i - 1) * (seq_t)(i - 1);  /* (i-1)^2 */
        term2 = term2 * (seq_t)2 * cache[i - 2];        /* 2*(i-1)^2*a(i-2) */
        cache[i] = term1 + term2;
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
