/*
 * A247903  Start with a single square; at n-th generation add a square at
 *          each expandable vertex (this is the "vertex to side" version);
 *          a(n) is the sum of all label values at n-th generation. (See
 *          comment for construction rules.)
 * https://oeis.org/A247903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+a(n-2)-4*a(n-3)+2*a(n-4); Initial values:
 *            a(0)=1, a(1)=5, a(2)=13, a(3)=29
 * Range:     u8 n <= 7, u16 n <= 22, u32 n <= 54, u64 n <= 118, u128 n <= 246,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)5;
    if (n == 2u) return (seq_t)13;
    if (n == 3u) return (seq_t)29;

    seq_t a0 = 1;   /* a(n-4)  */
    seq_t a1 = 5;   /* a(n-3)  */
    seq_t a2 = 13;  /* a(n-2)  */
    seq_t a3 = 29;  /* a(n-1)  */
    seq_t an = 0;

    for (idx_t i = 4; i <= n; ++i) {
        an = a3 * (seq_t)2          /* 2*a(n-1) */
          + a2                       /* + a(n-2) */
          + a0 * (seq_t)2;           /* + 2*a(n-4) */
        an = an - a1 * (seq_t)4;     /* - 4*a(n-3) */

        /* shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
