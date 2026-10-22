/*
 * A373081  The area of the n-th convex honeycomb formed by spirals of
 *          incrementing strips of hexagons.
 * https://oeis.org/A373081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 99*(a(n-5) - a(n-10)) + a(n-15) with initial values: a(1)
 *            = 1, a(2) = 3, a(3) = 10, a(4) = 21, a(5) = 91, a(6) = 120, a(7)
 *            = 300, a(8) = 990, a(9) = 2080, a(10) = 8911, a(11) = 11781,
 *            a(12) = 29403, a(13) = 97020, a(14) = 203841, a(15) = 873181
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

extern seq_t a(idx_t n);  /* forward declaration for reference */

seq_t a(idx_t n)
{
    /* Base values a(1) .. a(15) */
    static const seq_t base[16] = {
        0,            /* unused: index 0 */
        1, 3, 10, 21,          /* a(1) .. a(4) */
        91, 120, 300, 990,     /* a(5) .. a(8) */
        2080, 8911, 11781,     /* a(9) .. a(11) */
        29403, 97020, 203841,  /* a(12) .. a(14) */
        873181                  /* a(15) */
    };

    if (n <= 15) {
        return base[(size_t)n];
    }

    /* Sliding window buffer of the last 15 computed values
       buffer[0] holds a(k-15), buffer[14] holds a(k-1)  */
    seq_t buf[15];
    for (size_t i = 1; i <= 15; ++i) {
        buf[i - 1] = base[i];
    }

    seq_t next = 0;
    for (idx_t k = 16; k <= n; ++k) {
        /* a(k-5) is buffer[10], a(k-10) is buffer[5], a(k-15) is buffer[0] */
        next = ((seq_t)99 * (buf[10] - buf[5])) + buf[0];

        /* Shift buffer left by one position and insert next */
        for (int i = 0; i < 14; ++i) {
            buf[i] = buf[i + 1];
        }
        buf[14] = next;
    }

    return next;
}
