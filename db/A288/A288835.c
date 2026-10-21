/*
 * A288835  a(n) = (1/2!)*3^n*(n+3)*(n).
 * https://oeis.org/A288835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n * (n+3) * n / 2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1, return 0 as a safe default */
    seq_t val = pow3(n);            /* 3^n */
    val *= (seq_t)n;                /* multiply by n */
    val *= (seq_t)(n + 3);          /* multiply by n+3 */
    val /= 2;                       /* division by 2, always exact */
    return val;
}
