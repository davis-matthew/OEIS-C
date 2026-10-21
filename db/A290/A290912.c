/*
 * A290912  a(n) = (1/6)*A290911(n).
 * https://oeis.org/A290912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 4*a(n-3) - a(n-4) Initial values: a(0) = 0,
 *            a(1) = 1, a(2) = 4, a(3) = 16
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t if needed */
/* idx_t and seq_t are defined by the build system. */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 4;
    if (n == 3) return 16;

    /* Variables holding a(n-4), a(n-3), a(n-2), a(n-1). */
    seq_t a0 = 0;          /* a(i-4)  */
    seq_t a1 = 1;          /* a(i-3)  */
    seq_t a2 = 4;          /* a(i-2)  */
    seq_t a3 = 16;         /* a(i-1)  */
    seq_t ai;              /* a(i)    */

    /* Compute forward until n. */
    for (idx_t i = 4; i <= n; ++i) {
        ai = (seq_t)4 * a3 + (seq_t)4 * a1 - a0;
        /* Shift the window forward. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }

    return a3; /* a3 now holds a(n) after the loop */
}
