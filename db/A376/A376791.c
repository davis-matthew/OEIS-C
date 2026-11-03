/*
 * A376791  Expansion of 1/sqrt((1 - x^3)^2 - 4*x).
 * https://oeis.org/A376791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binom(2*n-5*k, k) * binom(2*n-6*k,
 *            n-3*k)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;          /* floor(n/3) – last k to use */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t t1 = binomial(2*n - 5*k, k);
        seq_t t2 = binomial(2*n - 6*k, n - 3*k);
        sum += t1 * t2;
    }

    return sum;
}
