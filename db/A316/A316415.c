/*
 * A316415  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          5, 6 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + 4*a(n-3) + 2*a(n-4) - 3*a(n-5) -
 *            9*a(n-6) - 6*a(n-7) for n > 10 with initial values: a(1) = 4,
 *            a(2) = 5, a(3) = 14, a(4) = 27, a(5) = 66, a(6) = 180, a(7) =
 *            484, a(8) = 1261, a(9) = 3346, a(10) = 8912
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial values for indices 1 … 10 */
    static const seq_t init[10] = {
        4,    /* a(1) */
        5,    /* a(2) */
        14,   /* a(3) */
        27,   /* a(4) */
        66,   /* a(5) */
        180,  /* a(6) */
        484,  /* a(7) */
        1261, /* a(8) */
        3346, /* a(9) */
        8912  /* a(10) */
    };

    if (n <= 10)
        return init[n - 1];

    /* Buffer holds a(n-1), a(n-2), …, a(n-7) for the current n */
    seq_t buf[7];
    buf[0] = init[9]; /* a(10) */
    buf[1] = init[8]; /* a(9)  */
    buf[2] = init[7]; /* a(8)  */
    buf[3] = init[6]; /* a(7)  */
    buf[4] = init[5]; /* a(6)  */
    buf[5] = init[4]; /* a(5)  */
    buf[6] = init[3]; /* a(4)  */

    seq_t term = 0;

    for (idx_t i = 11; i <= n; ++i) {
        term =
            buf[0]                         /* a(n-1) */
          + (seq_t)3 * buf[1]              /* 3*a(n-2) */
          + (seq_t)4 * buf[2]              /* 4*a(n-3) */
          + (seq_t)2 * buf[3]              /* 2*a(n-4) */
          - (seq_t)3 * buf[4]              /* -3*a(n-5) */
          - (seq_t)9 * buf[5]              /* -9*a(n-6) */
          - (seq_t)6 * buf[6];             /* -6*a(n-7) */

        /* Shift buffer for next step */
        buf[6] = buf[5];
        buf[5] = buf[4];
        buf[4] = buf[3];
        buf[3] = buf[2];
        buf[2] = buf[1];
        buf[1] = buf[0];
        buf[0] = term;
    }

    return term;
}
