/*
 * A264067  Number of (n+1) X (4+1) arrays of permutations of 0..n*5+4 with
 *          each element having index change +-(.,.) 0,0 1,1 or 1,2.
 * https://oeis.org/A264067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 19*a(n-1) + 16*a(n-2) for n > 3
 * Initial values: a(1) = 34, a(2) = 605, a(3) = 12100
 */
#include <stddef.h>    /* for size_t */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* below offset, behaviour doesn't matter */

    if (n == 1) return (seq_t)34;
    if (n == 2) return (seq_t)605;
    if (n == 3) return (seq_t)12100;

    seq_t prev2 = (seq_t)605;   /* a(2) */
    seq_t prev1 = (seq_t)12100; /* a(3) */
    seq_t cur = 0;

    for (idx_t i = 4; i <= n; ++i) {
        cur = (seq_t)19 * prev1 + (seq_t)16 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
