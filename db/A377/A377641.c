/*
 * A377641  a(n) = 3^n + 2^(3*n + 1) - 2^(2*n + 1).
 * https://oeis.org/A377641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n + 2^(3*n+1) - 2^(2*n+1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* offset 1 – defined for n ≥ 1 */
    if (n == 0) return (seq_t)0;

    idx_t exp_2n1 = 2 * n + 1;   /* exponent for 2^(2*n+1) */
    idx_t exp_3n1 = 3 * n + 1;   /* exponent for 2^(3*n+1) */

    seq_t pow3n   = pow3(n);          /* 3^n   */
    seq_t pow2_2n1 = pow2(exp_2n1);   /* 2^(2*n+1) */
    seq_t pow2_3n1 = pow2(exp_3n1);   /* 2^(3*n+1) */

    seq_t res = pow3n + pow2_3n1;
    res -= pow2_2n1;
    return res;
}
