/*
 * A322417  a(n) - 2*a(n-1) = period 2: repeat [3, 0] for n > 0, a(0)=5,
 *          a(1)=13.
 * https://oeis.org/A322417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*2^n - 2 when n is even, 7*2^n - 1 when n is odd
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t pow = pow2(n);          /* 2^n */
    seq_t res = pow * (seq_t)7;   /* 7*2^n */
    if ((n & (idx_t)1) == 0) {
        res -= (seq_t)2;
    } else {
        res -= (seq_t)1;
    }
    return res;
}
