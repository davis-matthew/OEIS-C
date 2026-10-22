/*
 * A368176  Expansion of e.g.f. -log(1 - x * exp(2*x)).
 * https://oeis.org/A368176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} (2*k)^(n-k) * (k-1)! * binomial(n,k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   alloc_seq_array, big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/alloc_seq_array.h"

/* Cache of previously computed values */
static seq_t *cache = NULL;      /* cache entries */
static idx_t  cache_len = 0;     /* number of valid entries */
static size_t cache_cap = 0;     /* allocated capacity */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Grow cache if necessary */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = alloc_seq_array(new_cap);
        if (cache) {
            for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
            free(cache);                  /* free old cache */
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t m = cache_len; m <= n; ++m) {
        if (m == 0) {
            cache[0] = 0;
            continue;
        }

        seq_t result = 0;

        /* (k-1)! : start with 0! = 1 for k=1 */
        seq_t fact = 1;

        /* binomial(n,k) : start with C(n,1) = n */
        seq_t binom = (seq_t)n;

        for (idx_t k = 1; k <= m; ++k) {
            /* pow_val = (2*k)^(m-k) */
            seq_t base = (seq_t)(2 * k);
            seq_t exp  = (seq_t)(m - k);
            seq_t pow_val = big_pow(base, exp);

            /* term = pow_val * fact * binom */
            seq_t term = pow_val;
            term = term * fact;
            term = term * binom;

            result += term;

            /* Update fact for next k: (k)!  (since next k' = k+1 => (k')-1 = k) */
            if (k < m) {
                fact = fact * (seq_t)k;
            }

            /* Update binom for next k: C(n,k+1) = C(n,k) * (n-k)/(k+1) */
            if (k < m) {
                /* numerator = C(n,k) * (n-k) */
                seq_t num = binom * (seq_t)(n - k);
                /* denom = k+1 */
                binom = num / (seq_t)(k + 1);
            }
        }

        cache[m] = result;
    }

    cache_len = n + 1;
    return cache[n];
}
