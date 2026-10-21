/*
 * A292056  Wiener index of the n-Keller graph.
 * https://oeis.org/A292056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n * (4^n + 3^n + n - 2) / 2 (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef _pow3_h
#include "../utils/pow3.h"   /* pow3(idx_t exp) */
#endif
#ifndef _pow4_h
#include "../utils/pow4.h"   /* pow4(idx_t exp) */
#endif

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n = 0 return 0
       (undefined in OEIS, but harmless for internal use). */
    if (n == 0) return 0;

    /* 4^n and 3^n as seq_t values */
    seq_t pow4n = pow4(n);
    seq_t pow3n = pow3(n);

    /* Since 4^n is a multiple of 4, the division by 2 is exact. */
    seq_t half_pow4n = pow4n / 2;

    /* Compute the inner sum: 4^n + 3^n + n – 2   */
    seq_t sum = pow4n + pow3n + (seq_t)n - 2;

    /* Final result: (4^n / 2) * (4^n + 3^n + n – 2) */
    return half_pow4n * sum;
}
