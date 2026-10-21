/*
 * A285309  Sum of nonsquare divisors of n.
 * https://oeis.org/A285309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_of_divisors(n) - sum_of_square_divisors(n) where a(n)
 *            = sum of divisors d of n for which d is not a perfect square.
 *            Implementation uses prime factorization and multiplicative
 *            formulas
 * Range:     u8 n <= 119, u16 n <= 9809, u32 n <= 9809, u64 n <= 9809,
 *            u128 n <= 9809, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* offset is 1 */

    /* factorize n */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t sigma = 1;                 /* sum of all divisors */
    seq_t sum_squares = 1;           /* sum of divisor squares that are perfect squares */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        idx_t e = exps[i];

        /* term = 1 + p + p^2 + ... + p^e */
        seq_t term = 1;
        seq_t p_pow = 1;
        for (idx_t k = 1; k <= e; ++k) {
            p_pow *= p;
            term += p_pow;
        }
        sigma *= term;

        /* term_sq = 1 + p^2 + p^4 + ... + p^(2*floor(e/2)) */
        idx_t m = e >> 1;            /* floor(e/2) */
        seq_t term_sq = 1;
        seq_t p_sq_pow = 1;
        for (idx_t k = 1; k <= m; ++k) {
            p_sq_pow *= p * p;
            term_sq += p_sq_pow;
        }
        sum_squares *= term_sq;
    }

    return sigma - sum_squares;
}
