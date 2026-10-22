/*
 * A381505  Expansion of e.g.f. exp(2*x/3) / (1-3*x)^(1/9).
 * https://oeis.org/A381505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1; for n>1 a(n) = (3*n-2)*a(n-1) - 2*(n-1)*a(n-2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the offset directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* Two previous values */
    seq_t a_prev2 = (seq_t)1; /* a(0) */
    seq_t a_prev1 = (seq_t)1; /* a(1) */
    seq_t a_curr  = (seq_t)0;

    /* Iterate from 2 up to n */
    for (idx_t i = 2; i <= n; ++i) {
        a_curr  = (seq_t)((3 * i) - 2) * a_prev1
                 - (seq_t)(2 * (i - 1)) * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_prev1;
}
