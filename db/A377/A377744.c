/*
 * A377744  E.g.f. satisfies A(x) = exp(x) / (1 - x * A(x))^4.
 * https://oeis.org/A377744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..n} (k+1)^(n‑k‑1) * binomial(5k+3,k)/(n‑k)!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* factorial n! */
    seq_t fact_n = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact_n *= (seq_t)i;

    seq_t sum   = 0;
    seq_t ratio = 1;                      /* n!/(n-k)! for current k */

    for (idx_t k = 0; k < n; ++k) {
        /* term = ratio * (k+1)^(n-k-1) * binomial(5k+3, k) */
        seq_t pow_val = big_pow((seq_t)(k + 1), (seq_t)(n - k - 1));
        seq_t term = ratio;
        term *= pow_val;
        term *= binomial(5 * k + 3, k);
        sum += term;

        /* update ratio for next k: multiply by (n - k) */
        ratio *= (seq_t)(n - k);
    }

    /* term for k = n : n! * binomial(5n+3, n) / (n + 1) */
    seq_t term_n = fact_n * binomial(5 * n + 3, n);
    term_n /= (n + 1);
    sum += term_n;

    return sum;
}
