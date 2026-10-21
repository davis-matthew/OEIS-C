/*
 * A282122  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 430", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A282122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=11, a(2)=11, a(3)=1111, a(n)=101*a(n-2)-100*a(n-4)
 *            for n>3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the initial four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1 || n == 2) return (seq_t)11;
    if (n == 3) return (seq_t)1111;

    /* Recurrence needs only a(n‑2) and a(n‑4).  Use a circular buffer of size 4. */
    seq_t buf[4];
    buf[0] = (seq_t)1;
    buf[1] = (seq_t)11;
    buf[2] = (seq_t)11;
    buf[3] = (seq_t)1111;

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute a(i) = 101*a(i-2) - 100*a(i-4) */
        seq_t val = (seq_t)101 * buf[(i - 2) & 3] - (seq_t)100 * buf[(i - 4) & 3];
        buf[i & 3] = val;
    }
    return buf[n & 3];
}
