/*
 * A276677  Number of squares added at the n-th generation of a symmetric
 *          (with 45-degree angles), non-overlapping Pythagoras tree.
 * https://oeis.org/A276677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*2^floor(n/2) + 8*2^floor((n-1)/2) - (4*n+8) for n>=1,
 *            a(0) = 1
 * Range:     u8 n <= 9, u16 n <= 25, u32 n <= 57, u64 n <= 121, u128 n <= 249,
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

    seq_t t1 = pow2(n / 2);          /* 2^⌊n/2⌋ */
    seq_t t2 = pow2((n - 1) / 2);    /* 2^⌊(n-1)/2⌋ */

    seq_t res = (seq_t)6 * t1 + (seq_t)8 * t2;
    seq_t sub = (seq_t)4 * (seq_t)n + (seq_t)8;

    return res - sub;
}
