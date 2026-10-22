/*
 * A396689  Numbers k such that binomial(x + 4, 5) - binomial(x, 5) = k^2 for
 *          some integer x >= 5.
 * https://oeis.org/A396689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A396689
 *  Formula:  a(n) = 98*a(n-2) - a(n-4)   for n ≥ 5
 *  Initial values:  a(1)=21,  a(2)=119,  a(3)=2059,  a(4)=11661
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* offset starts at 1 – undefined otherwise */
        return (seq_t)0;

    /* Handle the first four terms directly. */
    if (n == 1) return (seq_t)21;
    if (n == 2) return (seq_t)119;
    if (n == 3) return (seq_t)2059;
    if (n == 4) return (seq_t)11661;

    /* Compute iteratively using the recurrence.
       Only four previous values are needed. */
    seq_t prev4 = (seq_t)21;     /* a(1)  */
    seq_t prev3 = (seq_t)119;    /* a(2)  */
    seq_t prev2 = (seq_t)2059;   /* a(3)  */
    seq_t prev1 = (seq_t)11661;  /* a(4)  */
    seq_t cur   = (seq_t)0;

    size_t i;
    for (i = 5; i <= (size_t)n; ++i) {
        cur = prev2 * ((seq_t)98) - prev4; /* a(i) = 98*a(i-2) - a(i-4) */
        /* rotate the window */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
