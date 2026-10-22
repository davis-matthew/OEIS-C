/*
 * A307457  Longest path length in the n-Apollonian network.
 * https://oeis.org/A307457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) - 2 for n > 1; a(1) = 3 Recurrence: a(n) =
 *            3*a(n-1) - 2*a(n-2) for n > 3
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined position; return 0 safely */
        return (seq_t)0;
    if (n == 1)
        return (seq_t)3;
    if (n == 2)
        return (seq_t)6;
    if (n == 3)
        return (seq_t)14;

    /* Start recurrence with a(2) and a(3) */
    seq_t a_prev2 = (seq_t)6;   /* a(2) */
    seq_t a_prev1 = (seq_t)14;  /* a(3) */
    seq_t a_curr = 0;

    for (idx_t i = 4; i <= n; ++i)
    {
        a_curr = (seq_t)3 * a_prev1 - (seq_t)2 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
