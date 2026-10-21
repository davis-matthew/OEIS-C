/*
 * A268148  A double binomial sum involving absolute values.
 * https://oeis.org/A268148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(4*n-1)*n*(2*n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t res = pow2(4*n - 1);          /* 2^(4*n-1) */
    res *= (seq_t)n;                    /* * n */
    res *= (seq_t)(2*n - 1);            /* * (2*n-1) */
    return res;
}
