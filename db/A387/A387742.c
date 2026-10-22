/*
 * A387742  a(n) = (1/2) * Sum_{k=0..n-1} binomial(4*n,4*k+2).
 * https://oeis.org/A387742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 3, a(2) = 28, a(n) = 12*a(n-1) + 64*a(n-2) for
 *            n > 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)28;

    /* recurrence: a(n) = 12*a(n-1) + 64*a(n-2)  */
    seq_t prev  = (seq_t)28;   /* a(n-1) */
    seq_t prev2 = (seq_t)3;    /* a(n-2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t cur = (seq_t)12 * prev + (seq_t)64 * prev2;
        prev2 = prev;
        prev  = cur;
    }
    return prev;
}
