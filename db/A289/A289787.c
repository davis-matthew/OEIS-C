/*
 * A289787  p-INVERT of the even positive integers (A005843), where p(S) = 1 -
 *          S - S^2.
 * https://oeis.org/A289787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 6*a(n-2) + 6*a(n-3) - a(n-4)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  with starting values: a(0)=2, a(1)=12, a(2)=62, a(3)=312   */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle first four terms directly */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)62;
    if (n == 3) return (seq_t)312;

    /* Iteratively compute subsequent terms (O(n) time, O(1) space) */
    seq_t v0 = (seq_t)2;     /* a(n-4)  */
    seq_t v1 = (seq_t)12;    /* a(n-3)  */
    seq_t v2 = (seq_t)62;    /* a(n-2)  */
    seq_t v3 = (seq_t)312;   /* a(n-1)  */
    seq_t cur = 0;

    for (idx_t i = 4; i <= n; ++i) {
        cur = (seq_t)6 * v3 - (seq_t)6 * v2 + (seq_t)6 * v1 - v0;
        /* shift the window for next iteration */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = cur;
    }

    return v3;
}
