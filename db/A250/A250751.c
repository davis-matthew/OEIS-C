/*
 * A250751  Number of (n+1) X (4+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)+x(i-1,j) in the j direction.
 * https://oeis.org/A250751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 90*3^n - 40*2^n + 13
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 16, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* a(0) is undefined, return 0 */

    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);   /* 3^n  (arbitrary‑precision if needed) */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);   /* 2^n  */

    seq_t term1 = pow3 * (seq_t)90;             /* 90*3^n  */
    seq_t term2 = pow2 * (seq_t)40;             /* 40*2^n  */

    return term1 - term2 + (seq_t)13;           /* final result */
}
