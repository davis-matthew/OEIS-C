/*
 * A257594  Consider the hexagonal lattice packing of circles; a(n) is the
 *          maximal number of circles that can be enclosed by a closed chain
 *          of n circles.
 * https://oeis.org/A257594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 0 for n<6, a(6)=1, and for n≥7 a(n) =
 *            a(n-1)+a(n-2)+a(n-6) – a(n-4) – a(n-5) (recurrence)
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* a(0) … a(5) are zero, a(6)=1 */
    if (n <= 5) {
        return 0;
    }
    /* a0..a5 hold a(i-6)…a(i-1) for the current i */
    seq_t a0 = 0;      /* a(i-6) */
    seq_t a1 = 0;      /* a(i-5) */
    seq_t a2 = 0;      /* a(i-4) */
    seq_t a3 = 0;      /* a(i-3) */
    seq_t a4 = 0;      /* a(i-2) */
    seq_t a5 = 1;      /* a(i-1), i=7 after first shift */

    if (n == 6) {
        return a5;    /* a(6) */
    }

    for (idx_t i = 7; i <= n; ++i) {
        seq_t next = a5 + a4 + a0 - a2 - a1; /* recurrence */
        /* shift window forward */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }

    return a5;
}
