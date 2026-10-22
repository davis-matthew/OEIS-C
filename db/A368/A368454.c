/*
 * A368454  Expansion of e.g.f. 2*exp(-x) / (3 - exp(2*x)).
 * https://oeis.org/A368454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n + Sum_{k=1..n} 2^(k-1) * binomial(n,k) * a(n-k)
 *            with a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache to keep already computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;   /* number of entries currently stored */

    /* grow cache if needed */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed, return zero */
    }
    if ((size_t)n >= cache_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values up to n if necessary */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t value;
        if (i == 0) {
            value = 1;               /* a(0) = 1 */
        } else {
            seq_t sum = 0;           /* accumulator for the recurrence */
            seq_t comb = 1;          /* binomial coefficient, start with C(i,0)=1 */
            seq_t power = 1;         /* 2^(k-1), initial value for k=1 */

            for (idx_t k = 1; k <= i; ++k) {
                /* update binomial coefficient C(i,k) from C(i,k-1) */
                if (k > 1) {
                    comb = comb * (seq_t)(i - k + 1) / (seq_t)k;
                } else {
                    comb = (seq_t)i;   /* C(i,1) = i */
                }

                /* update power of 2: 2^(k-1) */
                if (k > 1) {
                    power *= (seq_t)2;
                }

                /* contribution: 2^(k-1) * C(i,k) * a(i-k) */
                seq_t term = power * comb;
                term = term * cache[i - k];
                sum += term;
            }

            /* add (-1)^i term */
            if ((i & 1) == 0) {
                sum += (seq_t)1;
            } else {
                sum -= (seq_t)1;
            }
            value = sum;
        }
        cache[i] = value;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
