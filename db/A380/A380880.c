/*
 * A380880  Expansion of e.g.f. (1/x) * Series_Reversion( x * exp(-3*x*exp(x))
 *          ).
 * https://oeis.org/A380880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * Σ_{k=1..n} k^(n-k) * (3n+3)^(k-1) * binomial(n,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* cache[0..last] holds computed a(i) */
    static idx_t cache_len = 0;       /* number of valid elements in cache */

    /* Grow cache if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)malloc((size_t)new_len * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;            /* allocation failed */

        /* copy old values */
        if (cache_len > 0)
            memcpy(new_cache, cache, (size_t)cache_len * sizeof(seq_t));

        /* free old cache */
        free(cache);
        cache = new_cache;
        cache_len = new_len;

        /* compute new entries */
        for (idx_t i = (cache_len > 1 ? cache_len - (new_len - i) : 0); i < new_len; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
                continue;
            }

            seq_t sum = 0;
            seq_t base_3n3 = (seq_t)3 * (seq_t)i + (seq_t)3;

            for (idx_t k = 1; k <= i; ++k) {
                seq_t pow1 = big_pow((seq_t)k, (seq_t)(i - k));          /* k^(i-k)   */
                seq_t pow2 = big_pow(base_3n3, (seq_t)(k - 1));           /* (3i+3)^(k-1) */
                seq_t bin  = binomial(i, k);                               /* binomial(i,k) */

                seq_t term = pow1 * pow2 * bin;      /* k^(i-k) * (3i+3)^(k-1) * binomial(i,k) */
                sum += term;
            }
            cache[i] = sum * (seq_t)3;              /* multiplied by 3 */
        }
    }

    return cache[n];
}
