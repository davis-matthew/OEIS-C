/*
 * A250162  Number of length n+1 0..3 arrays with the sum of adjacent
 *          differences multiplied by some arrangement of +-1 equal to zero.
 * https://oeis.org/A250162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(2 - 3*2^n + 4^n + 2*n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 59,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t p2 = seq_pow((seq_t)2, n);        /* 2^n */
    seq_t p4 = seq_pow((seq_t)4, n);        /* 4^n */
    seq_t term = (seq_t)2 *
        ( (seq_t)2
          - (seq_t)3 * p2
          + p4
          + (seq_t)(2 * n) );
    return term;
}
