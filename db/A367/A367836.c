/*
 * A367836  Expansion of e.g.f. 1/(2 - x - exp(3*x)).
 * https://oeis.org/A367836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = n*a(n-1) + Sum_{k=1..n} 3^k * binomial(n,k) *
 *            a(n-k); (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>                     /* malloc, free */
#include "../utils/binomial.h"          /* binomial(n, k)     */
#include "../utils/pow3.h"              /* pow3(exp)          */

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;
    idx_t i;

    /* Ensure cache has space up to n */
    if (n >= cap) {
        /* Create a new bigger array */
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;           /* allocation failure */

        /* Copy existing values, if any */
        if (cache) {
            for (i = 0; i < cap; ++i)
                new_cache[i] = cache[i];
            free(cache);
        } else {
            /* First allocation: initialise a(0) */
            new_cache[0] = (seq_t)1;
        }

        cache = new_cache;
        cap   = new_cap;
    }

    /* Return the stored value if it exists */
    if (cache[n] != 0)
        return cache[n];

    /* Compute a(n) using the recurrence */
    seq_t sum = 0;
    seq_t n_seq = (seq_t)n;

    for (i = 1; i <= n; ++i) {
        seq_t comb   = binomial(n, i);      /* C(n,i)          */
        seq_t p3     = pow3(i);             /* 3^i             */
        seq_t prod   = comb * cache[n - i] * p3;
        sum += prod;
    }

    /* a(n) = n*a(n-1) + sum */
    cache[n] = n_seq * cache[n - 1] + sum;
    return cache[n];
}
