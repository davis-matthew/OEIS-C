/*
 * A396422  a(n) = r(n,n), where r(1,k)=f(k)=round(k*c), r(n,k)=f(r(n-1,k)),
 *          c=4/3.
 * https://oeis.org/A396422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = r(n,n), where r(1,k)=round(k*4/3) and
 *            r(n,k)=round(r(n-1,k)*4/3). The rounding is rounded to the
 *            nearest integer (ties go up)
 * Range:     u8 n <= 10, u16 n <= 27, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   _round_mul_c
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_round_mul_c.h"

/* Main function required by the platform. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined by OEIS, but safe */
    seq_t x = (seq_t)n;                    /* start with k = n */
    for (idx_t i = 0; i < n; ++i) {
        x = _round_mul_c(x);              /* apply the round each step */
    }
    return x;                              /* r(n,n) */
}
