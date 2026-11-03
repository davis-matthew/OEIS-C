/*
 * A292318  Expansion of Product_{k>=1} ((1 + k!*x^k)/(1 - k!*x^k)).
 * https://oeis.org/A292318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in Prod_{k>=1} (1 + k!*x^k)/(1 -
 *            k!*x^k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t len = (size_t)n + 1;

    /* allocate dp buffers */
    seq_t *dp      = mem_new(seq_t, len);
    seq_t *dp_next = mem_new(seq_t, len);
    if (!dp || !dp_next) {
        mem_free(dp); mem_free(dp_next);
        return 0;
    }
    for (size_t i = 0; i < len; ++i) dp[i] = 0;
    dp[0] = 1;

    /* pre‑compute factorials up to n */
    seq_t *fact = mem_new(seq_t, len);
    if (!fact) {
        mem_free(dp); mem_free(dp_next);
        return 0;
    }
    fact[0] = 1;
    for (size_t i = 1; i < len; ++i) {
        fact[i] = fact[i-1] * (seq_t)i;
    }

    for (idx_t k = 1; k <= n; ++k) {
        /* start with the case that k is not used */
        for (size_t i = 0; i < len; ++i) dp_next[i] = dp[i];

        seq_t pow = (seq_t)2 * fact[(size_t)k];            /* coeff for t = 1 */
        size_t max_t = (size_t)(n / k);
        for (size_t t = 1; t <= max_t; ++t) {
            size_t shift = (size_t)k * t;
            seq_t factor = pow;                            /* coeff for current t */
            for (size_t s = shift; s <= n; ++s) {
                dp_next[s] += dp[s - shift] * factor;
            }
            /* prepare factor for the next t (increase power of k!) */
            pow *= fact[(size_t)k];
        }

        /* swap dp and dp_next for the next iteration */
        seq_t *tmp = dp; dp = dp_next; dp_next = tmp;
    }

    seq_t result = dp[(size_t)n];
    mem_free(dp);
    mem_free(dp_next);
    mem_free(fact);
    return result;
}
