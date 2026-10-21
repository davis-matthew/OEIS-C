/*
 * A295718  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = 1,
 *          a(1) = 3, a(2) = 4, a(3) = 5.
 * https://oeis.org/A295718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4) Initial values:
 *            a(0) = 1, a(1) = 3, a(2) = 4, a(3) = 5
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 49, u64 n <= 95, u128 n <= 188,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)5;

    /* Keep the last four values in a simple circular buffer. */
    seq_t a0 = (seq_t)1; /* a(k-4) */
    seq_t a1 = (seq_t)3; /* a(k-3) */
    seq_t a2 = (seq_t)4; /* a(k-2) */
    seq_t a3 = (seq_t)5; /* a(k-1) */

    for (idx_t k = 4; k <= n; ++k) {
        /* Compute a(k) using the recurrence. */
        seq_t next = a3 + (seq_t)3 * a2
                     - (seq_t)2 * a1
                     - (seq_t)2 * a0;
        /* Shift the window for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3; /* a(n) */
}
