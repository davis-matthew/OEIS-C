/*
 * A261723  Interleave 2^n + 2 and 2^n + 1.
 * https://oeis.org/A261723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2n-1) = 2^n + 2, a(2n) = 2^n + 1. (a(1)=4, a(2)=3, …)
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 254, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* k = ceil(n/2) = (n+1)/2 */
    idx_t k = (n + 1) >> 1;          /* n is uint64_t, so this is safe */
    seq_t val = pow2(k);             /* compute 2^k in the proper integer type */

    if (n & 1)                       /* odd n -> 2n-1 case */
        return val + 2;
    else                            /* even n -> 2n case */
        return val + 1;
}
