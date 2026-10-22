/*
 * A390902  Expansion of e.g.f. Series_Reversion(4*x + log(1-3*x)).
 * https://oeis.org/A390902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = -3*a(n-1) + 6 * Sum_{k=1..n-1} binomial(n,k) *
 *            a(k) * a(n-k) (n ≥ 2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* 1‑based cache: cache[0] is unused                     */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;    /* allocated size in elements         */
    static idx_t   last  = 0;    /* highest index computed so far      */

    if (n < 1) return 0;         /* undefined for n < 1               */

    /* grow the cache if necessary                           */
    size_t needed = (size_t)n + 1;   /* include dummy slot 0             */
    if (needed > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) {               /* allocation failed – return 0       */
            free(cache);
            cache = NULL;
            cap   = 0;
            last  = 0;
            return 0;
        }
        cache = tmp;
        cap   = needed;
    }

    /* compute missing values up to n                            */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1) {              /* base case                         */
            cache[1] = 1;
            continue;
        }

        seq_t sum = 0;
        seq_t binom = 1;           /* C(i,0) = 1                        */

        /* iterate over k = 1 .. i-1                                 */
        for (idx_t k = 1; k < i; ++k) {
            /* binom = C(i,k) = C(i,k-1)*(i-k+1)/k */
            binom = (binom * ((seq_t)(i - k + 1))) / ((seq_t)k);

            /* term = binom * a(k) * a(i-k) */
            seq_t term = binom * cache[k];
            term = term * cache[i - k];
            sum += term;
        }

        /* a(i) = 6*sum - 3*a(i-1)                                   */
        seq_t val = sum * ((seq_t)6);
        val = val - cache[i - 1] * ((seq_t)3);
        cache[i] = val;
    }

    last = n;
    return cache[n];
}
