/*
 * A377113  a(n) = coefficient of the term that is independent of sqrt(2),
 *          sqrt(3), and sqrt(6) in the expansion of (3 + sqrt(2) +
 *          sqrt(3))^n.
 * https://oeis.org/A377113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 44*a(n-2) + 48*a(n-3) + 8*a(n-4) a(0) = 1,
 *            a(1) = 3, a(2) = 14, a(3) = 72
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle base cases first */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)14;
    if (n == 3) return (seq_t)72;

    /* Use only the last four computed values */
    seq_t a0 = (seq_t)1;   /* a(n-4) */
    seq_t a1 = (seq_t)3;   /* a(n-3) */
    seq_t a2 = (seq_t)14;  /* a(n-2) */
    seq_t a3 = (seq_t)72;  /* a(n-1) */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = 12 * a3 - 44 * a2 + 48 * a1 + 8 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return a3;
}
