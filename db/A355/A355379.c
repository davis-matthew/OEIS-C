/*
 * A355379  Expansion of e.g.f. exp(exp(3*x) + exp(x) - 2).
 * https://oeis.org/A355379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = Σ_{k=1..n} (3^k + 1) * binomial(n-1, k-1) *
 *            a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* memoisation array */
    static idx_t cache_cap = 0;        /* allocated size of cache   */
    static idx_t last_computed = 0;    /* largest n already stored   */

    /* enlarge cache if the requested n is outside the allocated region */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (cache != NULL) {
            memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* initialise a(0) on first use */
    if (last_computed == 0) {
        cache[0] = (seq_t)1;
        last_computed = 0;
    }

    /* compute all not-yet-computed values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;          /* C(i-1, 0) */
        seq_t pow3  = (seq_t)3;          /* 3^1     */

        for (idx_t k = 1; k <= i; ++k) {
            /* term = a(i-k) * (3^k + 1) * binomial(i-1, k-1) */
            seq_t term = cache[i - k];
            term *= (pow3 + (seq_t)1);
            term *= binom;
            sum  += term;

            /* update binomial coefficient and 3^k for next k */
            if (k < i) {
                seq_t numerator = (seq_t)(i - k);           /* i - k      */
                binom = (binom * numerator) / (seq_t)k;    /* C(i-1, k)  */
                pow3 *= (seq_t)3;                           /* 3^(k+1)    */
            }
        }
        cache[i] = sum;
        last_computed = i;
    }

    return cache[n];
}
