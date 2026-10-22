/*
 * A384635  Expansion of (1+3*x+2*x^2) / (1-6*x^2-4*x^3+2*x^4).
 * https://oeis.org/A384635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(0) = 1, a(1) = 3, a(2) = 8, a(3) = 22, a(n) = 6*a(n-2) +
 *            4*a(n-3) - 2*a(n-4) for n ≥ 4
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)3;
    if (n == (idx_t)2) return (seq_t)8;
    if (n == (idx_t)3) return (seq_t)22;

    /* Iterate from 4 up to n, keeping only five latest values. */
    seq_t a0 = (seq_t)1;   /* a(n-4) */
    seq_t a1 = (seq_t)3;   /* a(n-3) */
    seq_t a2 = (seq_t)8;   /* a(n-2) */
    seq_t a3 = (seq_t)22;  /* a(n-1) */
    seq_t a4 = 0;          /* a(n)   */

    for (idx_t i = (idx_t)4; i <= n; ++i) {
        a4 = a2 * (seq_t)6 + a1 * (seq_t)4 - a0 * (seq_t)2;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    return a3;
}
