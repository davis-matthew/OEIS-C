/*
 * A307996  Expansion of e.g.f. exp(1 - exp(x)*(1 - 2*x)).
 * https://oeis.org/A307996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A307996 : a(0)=1, a(n)=Σ_{k=1..n}(2*k-1)*binomial(n-1,k-1)*a(n-k)   */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /*  Static cache of computed terms.
        cache[i] holds a(i) for all i ≤ last_computed. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* allocated size of cache */
    static idx_t last_computed = 0;    /* highest index that has been computed */

    /* initialise cache with a(0)=1 on first call */
    if (cache_size == 0) {
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * 1);
        cache_size = 1;
        cache[0] = (seq_t)1;
    }

    /* extend the cache if necessary */
    if ((size_t)n + 1 > cache_size) {
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * ((size_t)n + 1));
        cache_size = (size_t)n + 1;
    }

    /* compute new terms up to n if they are missing */
    if (last_computed < n) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t sum   = (seq_t)0;
            seq_t binom = (seq_t)1;  /* binomial(i-1,0) */

            for (idx_t k = 1; k <= i; ++k) {
                seq_t weight = (seq_t)(2*k - 1);
                seq_t term   = binom * weight;      /* binom * weight */
                term         = term * cache[i - k]; /* * a(i-k) */
                sum          += term;

                /* advance to next binomial coefficient:
                   C(i-1,k) = C(i-1,k-1) * (i-k) / k                */
                if (k < i) {
                    binom = binom * (seq_t)(i - k) / (seq_t)k;
                }
            }
            cache[i] = sum;
        }
        last_computed = n;
    }

    return cache[n];
}
