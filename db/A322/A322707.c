/*
 * A322707  a(0)=0, a(1)=5 and a(n) = 22*a(n-1) - a(n-2) + 10 for n > 1.
 * https://oeis.org/A322707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*a(n-1) - a(n-2) + 10 (for n > 1) a(0) = 0 a(1) = 5
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the trivial cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)5;

    /* Iterative computation using a linear recurrence of order 2 */
    seq_t prev2 = (seq_t)0;   /* a(n-2) */
    seq_t prev1 = (seq_t)5;   /* a(n-1) */
    seq_t cur   = 0;          /* a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        cur   = 22 * prev1 - prev2 + 10;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
