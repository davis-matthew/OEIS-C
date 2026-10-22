/*
 * A389121  Lower (2/3)-midsequence of A000032 (Lucas numbers) and A000045
 *          (Fibonacci numbers); see Comments.
 * https://oeis.org/A389121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A389121 – Lower (2/3)-midsequence of Lucas and Fibonacci numbers
   Formula: a(n) = a(n-1) + a(n-2) + a(n-8) - a(n-9) - a(n-10)
   Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=4, a(4)=6,
                   a(5)=10, a(6)=17, a(7)=28, a(8)=45, a(9)=73
   The recurrence is valid for n >= 10.  This implementation
   evaluates the sequence iteratively, using a small fixed‑size
   circular buffer, so it works for any integer type seq_t,
   including arbitrary precision integer classes.
*/

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base values for 0 <= n < 10 */
    static const seq_t base[10] = {
        1, 1, 2, 4, 6, 10, 17, 28, 45, 73
    };

    if (n < 10)                   /* Exact values for the first ten terms */
        return base[(size_t)n];

    /* Sliding window of the last 10 values */
    seq_t buf[10];
    for (size_t i = 0; i < 10; ++i)
        buf[i] = base[i];

    /* Compute terms from 10 up to n */
    for (size_t i = 10; i <= (size_t)n; ++i)
    {
        /* buf[0] = a(i-10), buf[1] = a(i-9), ..., buf[9] = a(i-1) */
        seq_t next = buf[9] + buf[8] + buf[2] - buf[1] - buf[0];

        /* Shift left: drop a(i-10), make room for a(i) */
        for (size_t j = 0; j < 9; ++j)
            buf[j] = buf[j + 1];
        buf[9] = next;
    }

    return buf[9];                /* buf[9] now holds a(n) */
}
