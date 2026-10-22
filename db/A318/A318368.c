/*
 * A318368  a(n) = Sum_{d|n} (-1)^(n/d+1)*d*2^(d-1).
 * https://oeis.org/A318368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (-1)^(n/d+1) * d * 2^(d-1)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sumPos = 0;
    seq_t sumNeg = 0;

    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0)
            continue;

        /* divisor d */
        seq_t term1 = pow2(d - 1) * (seq_t)d;
        /* sign: positive when (n/d) is odd */
        if ((n / d) & 1ULL)
            sumPos += term1;
        else
            sumNeg += term1;

        idx_t e = n / d;
        if (e == d)   /* perfect square; already counted */
            continue;

        /* paired divisor e */
        seq_t term2 = pow2(e - 1) * (seq_t)e;
        /* sign for e: positive when (n/e) is odd, i.e., d is odd */
        if (d & 1ULL)
            sumPos += term2;
        else
            sumNeg += term2;
    }

    return sumPos - sumNeg;
}
