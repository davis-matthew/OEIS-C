/*
 * A378804  a(n) = n * 2^n * binomial(4*n, n).
 * https://oeis.org/A378804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * 2^n * binomial(4*n, n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    seq_t pow = pow2(n);                 // 2^n
    seq_t bin = binomial(4 * n, n);      // binomial(4n, n)
    seq_t prod = (seq_t)n * pow;         // n * 2^n
    return prod * bin;                   // * binomial(4n, n)
}
