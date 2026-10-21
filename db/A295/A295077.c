/*
 * A295077  a(n) = 2*n*(n-1) + 2^n - 1.
 * https://oeis.org/A295077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(n-1) + 2^n - 1, n >= 0
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;
    seq_t tn  = (seq_t)n;
    seq_t tnn1 = tn - (seq_t)1;
    seq_t term = (seq_t)2 * tn * tnn1;    /* 2*n*(n-1) */
    seq_t pown = pow2(n);                 /* 2^n */
    return term + pown - (seq_t)1;
}
