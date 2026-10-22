/*
 * A341705  a(n) = 70*binomial(n,8) + 2*binomial(n,4) + 1.
 * https://oeis.org/A341705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 70*binomial(n,8) + 2*binomial(n,4) + 1
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 38, u64 n <= 570,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t c8 = binomial(n, 8);
    seq_t c4 = binomial(n, 4);
    return (70 * c8) + (2 * c4) + 1;
}
