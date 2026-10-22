/*
 * A387749  a(n) = (1/7) * Sum_{k=0..n-1} binomial(7*n,7*k+2).
 * https://oeis.org/A387749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=3, a(2)=299, a(3)=44927
   a(n) = 71*a(n-1) + 7585*a(n-2) - 36991*a(n-3) - 128*a(n-4)   for n > 3 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)299;
    if (n == 3) return (seq_t)44927;

    /* iterative computation using only the last four values */
    seq_t p0 = (seq_t)0;    /* a(n-4)  */
    seq_t p1 = (seq_t)3;    /* a(n-3)  */
    seq_t p2 = (seq_t)299;  /* a(n-2)  */
    seq_t p3 = (seq_t)44927;/* a(n-1)  */
    seq_t cur = 0;

    for (idx_t i = 4; i <= n; ++i)
    {
        cur = 71 * p3 + 7585 * p2 - 36991 * p1 - 128 * p0;
        /* shift the window */
        p0 = p1;
        p1 = p2;
        p2 = p3;
        p3 = cur;
    }
    return cur;
}
