/*
 * A324172  Number of subsets of {1,...,n} that cross their complement.
 * https://oeis.org/A324172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - n^2 + n - 2 (a(0)=0)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    
    seq_t N = (seq_t)n;
    seq_t p = pow2(n);          /* 2^n                           */
    return p - N * N + N - (seq_t)2;
}
