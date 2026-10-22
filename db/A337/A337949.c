/*
 * A337949  a(n) = 2^(n*(n-1)/2) + 2^(n*(n+1)/2) for n > 0, with a(0) = 1.
 * https://oeis.org/A337949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n*(n-1)/2) + 2^(n*(n+1)/2) (a(0) = 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"           /* provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                  /* a(0) = 1 */

    /* Compute the two exponents, guaranteeing no underflow. */
    idx_t e1 = n * (n - 1) / 2;     /* n*(n-1)/2 */
    idx_t e2 = n * (n + 1) / 2;     /* n*(n+1)/2 */

    seq_t term1 = pow2(e1);         /* 2^e1 */
    seq_t term2 = pow2(e2);         /* 2^e2 */

    return term1 + term2;           /* a(n) = term1 + term2 */
}
