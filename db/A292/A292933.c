/*
 * A292933  E.g.f.: x/(x+3-2*exp(x)).
 * https://oeis.org/A292933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*a(n-1) + n*(n-1)*a(n-2) + 2*∑_{m=3}^n C(n,m)*a(n-m)
 *            (with a(0)=0, a(1)=1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of values currently stored   */
static size_t cache_cap  = 0;   /* current capacity of the cache       */

seq_t a(idx_t n)
{
    /* Ensure the cache holds at least n+1 entries */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* out of memory – return 0 safely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base cases if not yet present */
    if (cache_size == 0) {
        cache[0] = 0;
        cache[1] = 1;
        cache_size = 2;
    }

    /* Compute terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) { cache[0] = 0; continue; }
        if (i == 1) { cache[1] = 1; continue; }

        seq_t term = (seq_t)i * cache[i - 1];                /* n*a(n-1)         */
        term += (seq_t)i * (i - 1) * cache[i - 2];           /* n*(n-1)*a(n-2)   */

        if (i >= 3) {
            /* Compute C(i,3) first */
            seq_t binom = (seq_t)i * (i - 1) * (i - 2) / 6;
            for (idx_t m = 3; m <= i; ++m) {
                term += 2 * binom * cache[i - m];            /* 2*C(i,m)*a(i-m)  */
                if (m < i)
                    binom = binom * (i - m) / (m + 1);       /* next binomial   */
            }
        }

        cache[i] = term;
    }
    cache_size = (size_t)n + 1;

    return cache[n];
}
