/*
 * A331363  Pairs of coordinates of the corners in a counterclockwise
 *          triangular spiral.
 * https://oeis.org/A331363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = 2*a(n-6) - a(n-12) for n > 12 Initial values: a(1)=0,
 *            a(2)=0, a(3)=1, a(4)=0, a(5)=0, a(6)=1, a(7)=-2, a(8)=-1,
 *            a(9)=3, a(10)=-1, a(11)=0, a(12)=2
 * Range:     i8 n <= 66, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* first 12 non‑zero terms (index 0 unused) */
    const seq_t init[13] = { 0, 0, 0, 1, 0, 0, 1, -2, -1, 3, -1, 0, 2 };

    if (n <= 12) {
        return init[(idx_t)n];
    }

    seq_t buf[13];
    /* copy initial values into the circular buffer */
    for (idx_t i = 1; i <= 12; ++i) {
        buf[(int)i] = init[(int)i];
    }

    for (idx_t i = 13; i <= n; ++i) {
        int idx  = (int)(i % 13);
        int idx6 = (int)((i - 6) % 13);
        int idx12 = (int)((i - 12) % 13);
        buf[idx] = ((seq_t)2 * buf[idx6]) - buf[idx12];
    }

    return buf[(int)(n % 13)];
}
