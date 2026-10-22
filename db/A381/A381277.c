/*
 * A381277  Expansion of e.g.f. exp(sinh(3*x) / 3).
 * https://oeis.org/A381277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⎣(n-1)/2⎦} 9^k * binomial(n-1,2k) * a(n-2k-1)
 *            a(0) = 1 Recurrence taken from OEIS A381277
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Static cache of computed terms.  cache_size is the number of
       terms that have been computed (i.e. the highest index+1).  */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of terms stored */
    static size_t cache_cap = 0;          /* allocated capacity */

    /* Ensure the cache is allocated and large enough. */
    if (cache == NULL) {
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        cache_cap = n + 1;
    } else if ((size_t)(n + 1) > cache_cap) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        cache_cap = n + 1;
    }

    /* Compute missing terms up to 'n'.  */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
        } else {
            idx_t k_limit = (i - 1) / 2;
            seq_t sum = 0;
            seq_t pow9 = 1;  /* 9^k, starting with k=0 */

            for (idx_t k = 0; k <= k_limit; ++k) {
                seq_t bin   = binomial(i - 1, 2 * k);
                seq_t term  = cache[i - 2 * k - 1];
                term *= bin;           /* term *= binomial(n-1,2k) */
                term *= pow9;          /* term *= 9^k */
                sum += term;           /* accumulate */

                pow9 *= 9;             /* next power of 9 */
            }
            cache[i] = sum;
        }
    }
    cache_size = n + 1;   /* we now have values up to index n */

    return cache[n];
}
