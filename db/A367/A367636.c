/*
 * A367636  a(n) is the number of distinct combinations that can be created by
 *          painting the sections on a shape with n divisions that rotates
 *          around its center and consists of 4 identical arms at 90-degree
 *          intervals.
 * https://oeis.org/A367636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(4n-5) + 2^(2n-3) + 2^(n-1) for n ≥ 2, a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility to compute integer powers of 2. */
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset not used, but safe */
    if (n == 1) return (seq_t)2;          /* special‐case for n=1 */

    idx_t e1 = 4 * n - 5;                  /* exponent of 2^(4n-5) */
    idx_t e2 = 2 * n - 3;                  /* exponent of 2^(2n-3) */
    idx_t e3 = n - 1;                      /* exponent of 2^(n-1) */

    seq_t term1 = pow_int((seq_t)2, e1);
    seq_t term2 = pow_int((seq_t)2, e2);
    seq_t term3 = pow_int((seq_t)2, e3);

    return term1 + term2 + term3;
}
