/*
 * A394960  a(n) = (2^(n-1)-1) * (2^n-3).
 * https://oeis.org/A394960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n - 2)*(2^n - 3)/2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t two_n = pow2(n);              /* 2^n */
    seq_t term1 = two_n - 2;            /* 2^n - 2 */
    seq_t term2 = two_n - 3;            /* 2^n - 3 */
    seq_t prod   = term1 * term2;       /* (2^n - 2)(2^n - 3) */
    return prod / 2;                    /* divide by 2 -> binomial result */
}
