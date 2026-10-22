/*
 * A381165  a(n) = Sum_{k=0..n} binomial(2*n,n)*binomial(n,
 *          k)*(5*k)!/((k!)^3*(2*k)!).
 * https://oeis.org/A381165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2n, n) * Σ_{k=0..n} binomial(n, k) * *
 *            binomial(5k, 2k) * binomial(3k, k) * binomial(2k, k)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the multinomial identity
 * (5k)!/((k!)^3*(2k)!) = binomial(5k, 2k)*binomial(3k, k)*binomial(2k, k). */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = binomial(n, k);
        term *= binomial(5 * k, 2 * k);
        term *= binomial(3 * k, k);
        term *= binomial(2 * k, k);
        sum += term;
    }

    return binomial(2 * n, n) * sum;
}
