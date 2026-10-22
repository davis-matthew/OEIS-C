/*
 * A306847  a(n) = Sum_{k=0..floor(n/6)} binomial(n,6*k).
 * https://oeis.org/A306847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/6⌋} binomial(n,6*k)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* For n ≤ 5 the recurrence does not hold; the sequence is defined as 1. */
    if (n <= 5) return (seq_t)1;

    /* Sliding window of the last five terms.
       buf[0] … buf[5] hold a(i‑5) … a(i) during the iteration.   */
    seq_t buf[6];
    for (size_t i = 0; i < 6; ++i) buf[i] = (seq_t)1;

    for (idx_t i = 6; i <= n; ++i) {
        seq_t v = (seq_t)6  * buf[5];   /* 6*a(i-1) */
        v -= (seq_t)15 * buf[4];       /* -15*a(i-2) */
        v += (seq_t)20 * buf[3];       /* +20*a(i-3) */
        v -= (seq_t)15 * buf[2];       /* -15*a(i-4) */
        v += (seq_t)6  * buf[1];       /* +6*a(i-5) */

        /* shift the window to make room for the new value */
        for (size_t j = 0; j < 5; ++j) buf[j] = buf[j + 1];
        buf[5] = v;
    }

    return buf[5];
}
