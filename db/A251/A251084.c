/*
 * A251084  Number of (n+1) X (4+1) 0..2 arrays with no 2 X 2 subblock having
 *          the sum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 13*a(n-2) + 10*a(n-3) + 5*a(n-4) - 14*a(n-5) +
 *            9*a(n-6) - 2*a(n-7) for n > 8, with initial values: a(1)=420,
 *            a(2)=1306, a(3)=2362, a(4)=4718, a(5)=8512, a(6)=15780,
 *            a(7)=28616, a(8)=52934. All operations are performed in seq_t
 * Range:     u8 none, u16 n <= 8, u32 n <= 24, u64 n <= 56, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle indices below 1 in case */
    if (n == 0) return 0;

    /* first eight terms */
    static const seq_t init[8] = {
        420, 1306, 2362, 4718,
        8512, 15780, 28616, 52934
    };

    if (n <= 8) return init[n-1];

    /* buffer holding a(n-7) .. a(n-1) */
    seq_t buf[7];
    buf[0] = init[1];  /* a(2) */
    buf[1] = init[2];  /* a(3) */
    buf[2] = init[3];  /* a(4) */
    buf[3] = init[4];  /* a(5) */
    buf[4] = init[5];  /* a(6) */
    buf[5] = init[6];  /* a(7) */
    buf[6] = init[7];  /* a(8) */

    /* iterate from n = 9 up to desired n */
    for (idx_t i = 9; i <= n; ++i) {
        seq_t next = 6 * buf[6];          /* 6*a(n-1) */
        next -= 13 * buf[5];              /* -13*a(n-2) */
        next += 10 * buf[4];              /* +10*a(n-3) */
        next += 5  * buf[3];              /* +5*a(n-4) */
        next -= 14 * buf[2];              /* -14*a(n-5) */
        next += 9  * buf[1];              /* +9*a(n-6) */
        next -= 2  * buf[0];              /* -2*a(n-7) */

        /* shift buffer left and insert new term */
        for (int k = 0; k < 6; ++k) buf[k] = buf[k+1];
        buf[6] = next;
    }

    return buf[6];  /* a(n) */
}
