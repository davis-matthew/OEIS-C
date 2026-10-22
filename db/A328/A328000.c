/*
 * A328000  a(n) = Sum_{k=0..n}(k!*(n - k)!)/(floor(k/2)!*floor((n -
 *          k)/2)!)^2.
 * https://oeis.org/A328000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-5)*(n*(n+2)+32) if n even, 2^(n-1)*(n+1) if n odd
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 57, u128 n <= 119,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the first five terms explicitly to avoid
       negative exponents in the power-of-two computation. */
    if (n < 5) {
        static const seq_t small[5] = { (seq_t)1, (seq_t)2, (seq_t)5,
                                       (seq_t)16, (seq_t)28 };
        return small[n];
    }

    if (n & 1UL) {                 /* n is odd */
        seq_t p = pow2(n-1);       /* 2^(n-1) */
        return p * ((seq_t)n + (seq_t)1);
    } else {                       /* n is even */
        seq_t p = pow2(n-5);       /* 2^(n-5) */
        seq_t t = (seq_t)n * ((seq_t)n + (seq_t)2) + (seq_t)32;
        return p * t;
    }
}
