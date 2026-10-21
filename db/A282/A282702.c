/*
 * A282702  a(n) = 3*a(n-1) + a(n-2), with a(0)=4, a(1)=11.
 * https://oeis.org/A282702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-2) with a(0) = 4, a(1) = 11
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the two base cases */
    if (n == 0)
        return (seq_t)4;
    if (n == 1)
        return (seq_t)11;

    /* iteratively compute the recurrence using two previous terms */
    seq_t prev2 = (seq_t)4;   /* a(0) */
    seq_t prev1 = (seq_t)11;  /* a(1) */
    seq_t curr  = prev1;      /* placeholder for a(n) */
    for (idx_t i = 2; i <= n; ++i) {
        curr = prev1 * (seq_t)3 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
