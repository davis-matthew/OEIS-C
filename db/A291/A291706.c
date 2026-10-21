/*
 * A291706  Number of connected dominating sets in the n-ladder graph.
 * https://oeis.org/A291706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=3, a(2)=9, a(3)=24, a(4)=56, a(n)=2*a(n-1)+a(n-2) for n>4
 *            (equivalently a(n)=4*lucas(n-1,2) for n>2)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)24;
    if (n == 4) return (seq_t)56;

    seq_t prev2 = (seq_t)24;   /* a(3) */
    seq_t prev1 = (seq_t)56;   /* a(4) */
    seq_t cur   = 0;

    for (idx_t i = 5; i <= n; ++i) {
        cur = prev1 * (seq_t)2 + prev2;  /* a(i) */
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
