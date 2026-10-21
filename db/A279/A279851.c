/*
 * A279851  Number of n X 2 0..2 arrays with no element unequal to a strict
 *          majority of its horizontal and vertical neighbors, with the
 *          exception of exactly one element, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A279851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 61, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) + a(n-2) - 7*a(n-3) + 4*a(n-5)
   Initial values:
   a(1) = 0,  a(2) = 4,  a(3) = 10,  a(4) = 24,  a(5) = 54 */
#include <stdint.h>
seq_t a(idx_t n) {
    /* Handle indices below the offset */
    if (n <= 0) return (seq_t)0;

    /* Initial values for n = 1..5 */
    if (n <= 5) {
        /* 0, 4, 10, 24, 54 */
        static const seq_t init[5] = {0, 4, 10, 24, 54};
        return init[n - 1];
    }

    /* Use a circular buffer for the last five terms:
       buffer[0] = a(i-5), buffer[1] = a(i-4), buffer[2] = a(i-3),
       buffer[3] = a(i-2), buffer[4] = a(i-1)    */
    seq_t buffer[5] = {0, 4, 10, 24, 54}; /* a(1)..a(5) */
    idx_t i;
    for (i = 6; i <= n; ++i) {
        /* Compute a(i) using the recurrence */
        seq_t val = (seq_t)3 * buffer[4]          /* a(i-1) */
                + buffer[3]                      /* a(i-2) */
                - (seq_t)7 * buffer[2]           /* a(i-3) */
                + (seq_t)4 * buffer[0];          /* a(i-5) */

        /* Shift buffer left: drop a(i-5), move others left, insert val at the end */
        buffer[0] = buffer[1];
        buffer[1] = buffer[2];
        buffer[2] = buffer[3];
        buffer[3] = buffer[4];
        buffer[4] = val;
    }
    return buffer[4]; /* a(n) after loop */
}
