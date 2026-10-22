/*
 * A340498  Where 2^n appears in A340488 for the first time.
 * https://oeis.org/A340498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0) = 3; a(n) = (8 + 10*4^(n-1))/3 for n >= 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)3;

    /* 4^(n-1) = 2^(2*(n-1)) */
    idx_t exp = 2 * (n - 1);
    seq_t pow4 = pow2(exp);          /* compute 4^(n-1) */
    seq_t t = (seq_t)10 * pow4;      /* 10 * 4^(n-1) */
    t += (seq_t)8;                   /* +8 */
    return t / (seq_t)3;             /* divide by 3 */
}
