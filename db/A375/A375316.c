/*
 * A375316  Expansion of (1 + x)/(1 - x^2*(1 + x)^4).
 * https://oeis.org/A375316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + 4*a(n-3) + 6*a(n-4) + 4*a(n-5) + a(n-6), with
 *            initial values a(0)=1, a(1)=1, a(2)=1, a(3)=5, a(4)=11, a(5)=19
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t base[6] = {1, 1, 1, 5, 11, 19};
    if (n < 6)
        return base[(size_t)n];

    /* Circular buffer holding the last six values:
       buf[0] = a(i-6), buf[1] = a(i-5), ..., buf[5] = a(i-1)   */
    seq_t buf[6] = {1, 1, 1, 5, 11, 19};

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = buf[4]                    /* a(i-2) */
                    + 4 * buf[3]               /* 4*a(i-3) */
                    + 6 * buf[2]               /* 6*a(i-4) */
                    + 4 * buf[1]               /* 4*a(i-5) */
                    + buf[0];                  /* a(i-6) */

        /* Shift buffer forward and append next */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = next;
    }

    return buf[5];   /* a(n) is now in buf[5] */
}
