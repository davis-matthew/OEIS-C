/*
 * A263944  Positive integers n such that (n+28)^3 - n^3 is a square.
 * https://oeis.org/A263944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 6*a(n-2) + a(n-3) for n > 3 with a(1) = 28,
 *            a(2) = 189, a(3) = 959 This function works for any seq_t
 *            (integer or arbitrary‑precision) and any idx_t (usually
 *            uint64_t)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The sequence is defined starting at offset 1. */
    if (n <= 0)
        return (seq_t)0;

    if (n == 1)
        return (seq_t)28;
    if (n == 2)
        return (seq_t)189;
    if (n == 3)
        return (seq_t)959;

    /* Keep the last three values in a rolling buffer. */
    seq_t prev3 = (seq_t)28;   /* a(n-3) */
    seq_t prev2 = (seq_t)189;  /* a(n-2) */
    seq_t prev1 = (seq_t)959;  /* a(n-1) */
    seq_t cur;                 /* a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        cur = (seq_t)6 * prev1 - (seq_t)6 * prev2 + prev3;
        /* shift the window forward */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
