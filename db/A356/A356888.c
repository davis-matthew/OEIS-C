/*
 * A356888  a(n) = ((n-1)^2 + 2)*2^(n-2).
 * https://oeis.org/A356888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n-1)^2 + 2)*2^(n-2)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 54, u128 n <= 116,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the two initial terms explicitly to avoid
       negative exponent when n=1 or n=2. */
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)3;

    /* For n >= 3 compute ((n-1)^2 + 2) * 2^(n-2). */
    seq_t n1   = (seq_t)(n - 1);
    seq_t part = n1 * n1 + (seq_t)2;    /* ((n-1)^2 + 2)       */
    seq_t pow  = pow2(n - 2);           /* 2^(n-2)              */
    return part * pow;                  /* product */
}
