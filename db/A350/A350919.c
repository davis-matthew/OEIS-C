/*
 * A350919  a(0) = 9, a(1) = 9, and a(n) = 3*a(n-1) - a(n-2) - 4 for n >= 2.
 * https://oeis.org/A350919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 9, a(1) = 9, a(n) = 3*a(n-1) - a(n-2) - 4  (n >= 2) */
#include <stdint.h>                      /* for uint64_t */

seq_t a(idx_t n)
{
    /* Handle the two base cases directly */
    if (n == 0 || n == 1) {
        return (seq_t)9;
    }

    /* Use two variables to keep the previous two terms. */
    seq_t prev2 = (seq_t)9;   /* a(0) */
    seq_t prev1 = (seq_t)9;   /* a(1) */
    seq_t cur;

    /* Iteratively compute from 2 up to n */
    for (idx_t i = 2; i <= n; ++i) {
        cur = 3 * prev1 - prev2 - 4;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;   /* a(n) */
}
