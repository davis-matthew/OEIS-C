/*
 * A246883  Expansion of Sum_{n>=0} x^n * Sum_{k=0..n} C(n,k)^2 * x^(3*k).
 * https://oeis.org/A246883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} C(n-3k, k)^2 (C = binomial coefficient)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 38, u64 n <= 73, u128 n <= 142,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 4;    /* upper bound for k */
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t N = n - 3 * k;
        seq_t c = binomial(N, k);
        sum += c * c;
    }
    return sum;
}
