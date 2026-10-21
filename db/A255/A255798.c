/*
 * A255798  Number of (n+2) X (5+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum 1 and no antidiagonal sum 2 and no row sum 0 and no
 *          column sum 3.
 * https://oeis.org/A255798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-6) - 4*a(n-12) for n>15 Initial values: a(1)=733,
 *            a(2)=340, a(3)=256, a(4)=268, a(5)=286, a(6)=290, a(7)=472,
 *            a(8)=630, a(9)=674, a(10)=748, a(11)=814, a(12)=866, a(13)=1540,
 *            a(14)=2190, a(15)=2414
 * Range:     u8 none, u16 n <= 30, u32 n <= 78, u64 n <= 174, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const seq_t init[15] = {
        733, 340, 256, 268, 286,
        290, 472, 630, 674, 748,
        814, 866, 1540, 2190, 2414
    };

    if (n <= 0)
        return 0;
    if (n <= 15)
        return init[n - 1];

    /* iterative computation using the recurrence */
    seq_t buf[12];
    /* buf holds a(i-12) .. a(i-1) for the current i */
    /* initialise for i = 16: a(4)..a(15) */
    for (int j = 0; j < 12; ++j)
        buf[j] = init[3 + j];   /* init[3] = a(4) */

    seq_t val = 0;
    for (idx_t i = 16; i <= n; ++i) {
        val = (seq_t)5 * buf[6] - (seq_t)4 * buf[0];
        /* shift buffer left by one: drop a(i-12), add a(i) */
        for (int j = 0; j < 11; ++j)
            buf[j] = buf[j + 1];
        buf[11] = val;
    }
    return val;
}
