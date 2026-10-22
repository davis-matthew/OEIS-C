/*
 * A396110  Number of digraphs on n labeled nodes such that every node is
 *          reachable from node 1.
 * https://oeis.org/A396110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 2^(n*(n-1)) - Sum_{k=1..n-1} binomial(n-1,k-1) * a(k) *
 *            2^((n-k)*(n-1)); a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (cache_len < n) {
        /* grow cache */
        idx_t new_cap = cache_len == 0 ? (idx_t)2 : cache_len * 2;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

        if (cache_len == 0) {          /* first call: initialize a(1) */
            cache[0] = 0;              /* unused */
            cache[1] = (seq_t)1;       /* a(1) = 1 */
            cache_len = 1;
        }

        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)1;
                continue;
            }

            seq_t total = pow2(i * (i - 1));               /* 2^(i*(i-1)) */
            seq_t sum   = 0;
            seq_t pow_term = pow2((i - 1) * (i - 1));      /* 2^((i-1)*(i-1)) */
            seq_t binom = (seq_t)1;                       /* binom(i-1, k-1) initially k=1 */

            for (idx_t k = 1; k <= i - 1; ++k) {
                seq_t term = binom * cache[k] * pow_term;
                sum += term;

                if (k < i - 1) {
                    /* update binomial coefficient for next k */
                    binom *= (seq_t)(i - k);
                    binom /= (seq_t)k;

                    /* update power of two for next k */
                    pow_term >>= (i - 1);
                }
            }

            cache[i] = total - sum;
        }
        cache_len = n;
    }

    return cache[n];
}
