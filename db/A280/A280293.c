/*
 * A280293  a(0) = 3, a(n+1) = 2*a(n) + periodic sequence of length 2: repeat
 *          [-5, 4].
 * https://oeis.org/A280293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 3, a(1) = 1, a(n+1) = 2*a(n) + p[n], where p repeats [-5,
 *            4] of period 2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A280293. */
#include <stdlib.h>

/* idx_t and seq_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* Handle the first two values explicitly. */
    if (n == 0)
        return (seq_t)3;
    if (n == 1)
        return (seq_t)1;

    /* Compute iteratively until the desired index is reached.
       Complexity O(n). */
    seq_t prev2 = (seq_t)3;   /* a(0) */
    seq_t prev1 = (seq_t)1;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        /* Determine the periodic adjustment:
           When (i-1) is even, use -5; otherwise use +4. */
        if (((i - 1) & 1U) == 0U)          /* (i-1) even */
            cur = (prev1 << 1) - (seq_t)5;
        else
            cur = (prev1 << 1) + (seq_t)4;

        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
