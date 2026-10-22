/*
 * A360730  Expansion of Sum_{k>=0} (k * x * (1 + k*x^2))^k.
 * https://oeis.org/A360730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} (n-2k)^(n-k) * binomial(n-2k, k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence implementation for OEIS A360730 */
#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    /* Accumulate the sum over k = 0..floor(n/3) */
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t m = n - 2 * k;                 /* n-2k */
        seq_t base = (seq_t)m;                /* base for power */
        seq_t exp  = (seq_t)(n - k);          /* exponent for power */

        /* (n-2k)^(n-k) */
        seq_t power = big_pow(base, exp);

        /* binomial(n-2k, k) */
        seq_t comb  = binomial(m, k);

        /* Multiply the two parts */
        seq_t term  = seq_mul(power, comb);

        /* Add to running total */
        sum += term;
    }
    return sum;
}
