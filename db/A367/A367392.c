/*
 * A367392  a(n) = Sum_{k=0..n} (-1)^k * binomial(n, k) * (n - k)^(2*n + 1).
 * https://oeis.org/A367392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * binomial(n, k) * (n - k)^(2*n + 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*            = n! * Stirling2(2*n + 1, n)                         */

#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    /* exponent 2*n + 1 */
    seq_t exp = (seq_t)(2 * n + 1);

    seq_t add_pos = (seq_t)0;
    seq_t add_neg = (seq_t)0;

    seq_t binom = (seq_t)1;          /* binomial(n, 0) */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t base = (seq_t)(n - k);
        seq_t power = big_pow(base, exp);      /* (n-k)^(2n+1) */
        seq_t term  = binom * power;           /* binomial * power */

        if ((k & 1) == 0)      /* even k -> positive term */
            add_pos += term;
        else                    /* odd k -> negative term */
            add_neg += term;

        /* update binomial for next k, unless we are at the last iteration */
        if (k < n) {
            /* binom = binom * (n - k) / (k + 1) */
            binom = binom * (seq_t)(n - k);
            binom = binom / (seq_t)(k + 1);
        }
    }

    /* final result: add_pos - add_neg (non‑negative) */
    return add_pos - add_neg;
}
