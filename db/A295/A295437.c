/*
 * A295437  a(n) = (18*n)!*n!/((9*n)!*(6*n)!*(4*n)!).
 * https://oeis.org/A295437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(18*n,9*n)*binomial(9*n,3*n)/binomial(4*n,n)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t n18 = 18 * n;
    idx_t n9  = 9 * n;
    idx_t n4  = 4 * n;
    idx_t n3  = 3 * n;

    seq_t part1 = binomial(n18, n9);
    seq_t part2 = binomial(n9, n3);
    seq_t part3 = binomial(n4, n);

    return (part1 * part2) / part3;
}
