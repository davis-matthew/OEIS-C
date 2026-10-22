/*
 * A397035  G.f. A(x) satisfies A(x) = 1 + x * A(x^2/(1 - x)^2) / (1 - x)^4.
 * https://oeis.org/A397035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0)=1;
 * a(n)=1+sum_{k=0..floor((n-1)/2)} binomial(n+2,2*k+3)*a(k)
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static idx_t   cached_max = 0;
    static size_t  cache_cap  = 0;
    static seq_t *cache      = NULL;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)1;                 /* a(0) = 1 */
        cached_max = 0;
    }

    /* return value if already computed */
    if (n <= cached_max) {
        return cache[n];
    }

    /* grow cache to hold up to index n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure */
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        idx_t upper = (i - 1) / 2;
        seq_t sum   = 0;
        idx_t N     = i + 2;                /* N = n + 2 in the formula */

        for (idx_t k = 0; k <= upper; ++k) {
            idx_t r   = 2 * k + 3;          /* lower index in binomial */
            seq_t bin = binomial(N, r);     /* helper provided by build */
            sum += bin * cache[k];
        }
        cache[i] = sum;
    }

    cached_max = n;
    return cache[n];
}
