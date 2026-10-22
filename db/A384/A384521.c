/*
 * A384521  Expansion of e.g.f. 5/(7 - 2*exp(5*x)).
 * https://oeis.org/A384521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A384521: Expansion of the e.g.f. 5/(7 - 2*exp(5*x))
   Formula: a(n) = 2 * Σ_{k=1..n} 5^(k-1) * binomial(n,k) * a(n-k)
   Initial value: a(0) = 1
   Generates a(n) for all n ≥ 0 using memoisation.
*/

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* static cache array for memoisation */
    static seq_t *cache  = NULL;  /* array of computed values a(0)..a(cache_len-1) */
    static size_t cache_cap = 0;  /* allocated capacity of cache */
    static size_t cache_len = 0;  /* number of values currently stored */

    /* Grow cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = (n + 1) * 2;
        if (new_cap < 64) new_cap = 64;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) { /* allocation failure – return 0 as fallback */
            return 0;
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialise a(0) if not yet done */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Compute missing terms up to n */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = 0;
            seq_t pow5 = (seq_t)1;  /* 5^(k-1) for current k */
            for (idx_t k = 1; k <= i; ++k) {
                if (k > 1) {
                    pow5 = pow5 * (seq_t)5;   /* update 5^(k-1) */
                }
                seq_t bin = binomial(i, k);
                seq_t prod = pow5 * bin;
                prod = prod * cache[i - k];
                sum = sum + prod;
            }
            cache[i] = sum * 2;  /* a(i) = 2 * sum */
        }
        cache_len = n + 1;
    }

    return cache[n];
}
