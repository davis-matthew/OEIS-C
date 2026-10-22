/*
 * A339771  a(n) = Sum_{i=0..n} Sum_{j=0..n} 2^max(i,j).
 * https://oeis.org/A339771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-1) * 2^(n+1) + 3
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 25, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t factor = ((seq_t)n * (seq_t)2) - (seq_t)1;
    seq_t power  = pow2(n + 1);
    return factor * power + (seq_t)3;
}
