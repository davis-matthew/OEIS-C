/*
 * A335749  a(n) = n!*[x^n] exp(2*x)*(y*sinh(x*y) + cosh(x*y)) and y =
 *          sqrt(6).
 * https://oeis.org/A335749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=8, a(n)=4*a(n-1)+2*a(n-2) for n≥2 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)8;   /* a(1) */
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i) {
        cur = 4 * prev1 + 2 * prev2; /* a(i) */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
