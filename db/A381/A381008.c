/*
 * A381008  Ordered perimeters of the Pythagorean triangles defined by a =
 *          2^(4n) + 2^(2n+1), b = 2^(4n) - 2^(4n-2) - 2^(2n) - 1, c = 2^(4n)
 *          + 2^(4n-2) + 2^(2n) + 1.
 * https://oeis.org/A381008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(4n+1) + 2^(2n+1) + 2^(4n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined for n=0 – harmless fallback */

    seq_t term1 = pow2(4*n + 1);   /* 2^(4n+1) */
    seq_t term2 = pow2(2*n + 1);   /* 2^(2n+1) */
    seq_t term3 = pow2(4*n);       /* 2^(4n)   */

    return term1 + term2 + term3;
}
