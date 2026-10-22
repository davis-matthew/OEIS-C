/*
 * A385689  a(n) = 6*binomial(n,4) + 6*binomial(n,3) + 4*binomial(n,2) + 2*n +
 *          1.
 * https://oeis.org/A385689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*binomial(n,4) + 6*binomial(n,3) + 4*binomial(n,2) + 2*n
 *            + 1
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    return (seq_t)6 * binomial(n, 4) +
           (seq_t)6 * binomial(n, 3) +
           (seq_t)4 * binomial(n, 2) +
           (seq_t)2 * n +
           1;
}
