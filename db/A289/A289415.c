/*
 * A289415  a(n) = ((11-sqrt(11))^n + (11+sqrt(11))^n) / 2.
 * https://oeis.org/A289415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=11, a(n)=22*a(n-1)-110*a(n-2) for n>1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;

    /* Iteratively compute up to n. */
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)11;  /* a(1) */
    seq_t cur   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)22 * prev1 - (seq_t)110 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
