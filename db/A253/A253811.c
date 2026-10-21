/*
 * A253811  Part of the y solutions of the Pell equation x^2 - 2*y^2 = +7.
 * https://oeis.org/A253811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2), a(0)=3, a(1)=19
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)19;

    seq_t prev2 = (seq_t)3;   /* a(n-2) */
    seq_t prev1 = (seq_t)19;  /* a(n-1) */
    seq_t curr;

    /* Compute iteratively using the recurrence */
    for (idx_t i = 2; i <= n; ++i) {
        curr = prev1 * (seq_t)6 - prev2;  /* a(n) = 6*a(n-1) - a(n-2) */
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
