/*
 * A377576  E.g.f. satisfies A(x) = (1 + x * exp(x) * A(x))^4.
 * https://oeis.org/A377576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} k^(n-k) * binomial(4*k+4,k) /
 *            ((k+1)*(n-k)!)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"    /* binomial(idx_t a, idx_t b) */
#include "../utils/seq_pow.h"     /* seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n) {
    /* Static cache of factorials and sequence values */
    static seq_t *fact = NULL;
    static size_t fact_cap = 0;
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Ensure factorial array covers 0..n */
    if (n + 1 > fact_cap) {
        seq_t *new_fact = (seq_t *)realloc(fact, (n + 1) * sizeof(seq_t));
        if (!new_fact) return 0; /* allocation fail, return 0 */
        fact = new_fact;
        if (fact_cap == 0) {
            fact[0] = 1;
            fact_cap = 1;
        }
        for (size_t i = fact_cap; i <= n; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_cap = n + 1;
    }

    /* Ensure value cache covers 0..n */
    if (n + 1 > cache_cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) return 0;
        cache = new_cache;
        /* Compute new terms from old cache_cap up to n */
        for (idx_t m = cache_cap; m <= n; ++m) {
            seq_t sum = 0;
            for (idx_t k = 0; k <= m; ++k) {
                /* P(m,k) = m!/(m-k)! = fact[m]/fact[m-k] */
                seq_t term = fact[m] / fact[m - k];

                /* k^(m-k) */
                seq_t pow_term;
                if (k == 0) {
                    pow_term = (m == 0) ? 1 : 0;
                } else {
                    pow_term = seq_pow((seq_t)k, m - k);
                }

                /* binomial(4*k+4, k) */
                seq_t bin = binomial(4 * k + 4, k);

                /* Multiply and divide by (k+1) */
                term = term * pow_term;
                term = term * bin;
                term = term / ((seq_t)(k + 1));

                sum += term;
            }
            cache[m] = sum;
        }
        cache_cap = n + 1;
    }

    return cache[n];
}
