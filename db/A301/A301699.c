/*
 * A301699  Generating function = g(g(x)), where g(x) = g.f. of Jacobsthal
 *          numbers A001045.
 * https://oeis.org/A301699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) - 6*a(n-3) - 4*a(n-4); a(0) = 0, a(1)
 *            = 1, a(2) = 2, a(3) = 8
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* initial terms */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)8;

    /* use a ring buffer of the last four values */
    seq_t a0 = (seq_t)0; // a(n-4)
    seq_t a1 = (seq_t)1; // a(n-3)
    seq_t a2 = (seq_t)2; // a(n-2)
    seq_t a3 = (seq_t)8; // a(n-1)

    for (idx_t i = 4; i <= n; ++i) {
        seq_t val = (seq_t)3 * a3 + (seq_t)4 * a2;
        val -= (seq_t)6 * a1;
        val -= (seq_t)4 * a0;
        /* shift window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = val;
    }
    return a3;
}
