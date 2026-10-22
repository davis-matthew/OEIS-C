/*
 * A390791  Number of Hamiltonian paths from vertex (1, 1) to (3, 2*n+2) on
 *          the product graph C_4 X P_{2*n+2}, using (row, column) numbering.
 * https://oeis.org/A390791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 21*a(n-1) - 26*a(n-2) + 44*a(n-3) - 8*a(n-4), * with
 *            initial values a(0)=6, a(1)=100, a(2)=1940, a(3)=38404
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return 6;
    if (n == 1) return 100;
    if (n == 2) return 1940;
    if (n == 3) return 38404;

    /* Use only a sliding window of the last four values. */
    seq_t a0 = 6;
    seq_t a1 = 100;
    seq_t a2 = 1940;
    seq_t a3 = 38404;
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        /* Recurrence: a(i) = 21*a(i-1) - 26*a(i-2) + 44*a(i-3) - 8*a(i-4) */
        next = 21 * a3;
        next -= 26 * a2;
        next += 44 * a1;
        next -= 8 * a0;

        /* Shift the window for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
