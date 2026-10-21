/*
 * A293069  Sum of values of vertices of type E at level n of the hyperbolic
 *          Pascal pyramid PP_(4,5).
 * https://oeis.org/A293069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) - 65*a(n-2) + 159*a(n-3) - 200*a(n-4) +
 *            122*a(n-5) - 28*a(n-6) initial values: a(0)=0, a(1)=0, a(2)=0,
 *            a(3)=0, a(4)=8, a(5)=70, a(6)=418
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Return the small pre‑computed values directly. */
    if (n <= 6) {
        static const seq_t init[7] = { 0, 0, 0, 0, 8, 70, 418 };
        return init[n];
    }

    /* last6[0] holds a(i-1), last6[1] holds a(i-2), … last6[5] holds a(i-6) */
    seq_t last6[6] = {
        (seq_t)418, /* a(6) */
        (seq_t)70,  /* a(5) */
        (seq_t)8,   /* a(4) */
        (seq_t)0,   /* a(3) */
        (seq_t)0,   /* a(2) */
        (seq_t)0    /* a(1) */
    };

    for (idx_t i = 7; i <= n; ++i) {
        seq_t val = (seq_t)13 * last6[0]
                  - (seq_t)65 * last6[1]
                  + (seq_t)159 * last6[2]
                  - (seq_t)200 * last6[3]
                  + (seq_t)122 * last6[4]
                  - (seq_t)28 * last6[5];

        /* shift the window: drop the oldest value, insert the newest */
        last6[5] = last6[4];
        last6[4] = last6[3];
        last6[3] = last6[2];
        last6[2] = last6[1];
        last6[1] = last6[0];
        last6[0] = val;
    }

    return last6[0]; /* a(n) */
}
