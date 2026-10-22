/*
 * A367835  Expansion of e.g.f. 1/(2 - x - exp(2*x)).
 * https://oeis.org/A367835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + Sum_{k=1..n} 2^k * binomial(n,k) * a(n-k);
 *            a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* static cache of already-computed terms */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;
    static idx_t    last  = 0;

    /* initialize cache on the first call */
    if (cache == NULL) {
        cap = 1;                                 /* store a(0) */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;       /* allocation failure guard */
        cache[0] = (seq_t)1;                     /* a(0) = 1 */
        last = 0;
    }

    /* grow the array if needed */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap;
        while ((size_t)n + 1 > newcap) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;       /* allocation failure guard */
        cache = tmp;
        cap = newcap;
    }

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sum   = (seq_t)0;
        seq_t pow2 = (seq_t)2;                /* 2^k, starts with k=1 */
        seq_t binom = (seq_t)i;                /* C(i,1) */

        for (idx_t k = 1; k <= i; ++k) {
            if (k > 1) {
                /* binomial coefficient update:
                   C(i,k) = C(i,k-1) * (i-k+1) / k
                */
                binom = binom * (seq_t)(i - k + 1) / (seq_t)k;
            }
            sum += binom * pow2 * cache[(size_t)(i - k)];
            pow2 = pow2 * (seq_t)2;           /* advance 2^(k+1) */
        }

        seq_t prod = (seq_t)i * cache[(size_t)(i - 1)];
        cache[(size_t)i] = prod + sum;
    }

    last = n;
    return cache[(size_t)n];
}
