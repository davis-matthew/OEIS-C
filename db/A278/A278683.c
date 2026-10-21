/*
 * A278683  Number of non-equivalent ways to place 4 non-attacking ferses on
 *          an n X n board.
 * https://oeis.org/A278683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 30, u64 n <= 493, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 4*a(n-1) - a(n-2) - 16*a(n-3) + 19*a(n-4) + 20*a(n-5)
           - 45*a(n-6) + 45*a(n-8) - 20*a(n-9) - 19*a(n-10)
           + 16*a(n-11) + a(n-12) - 4*a(n-13) + a(n-14)
   for n ≥ 17.
   Initial values:
   a(1)=0, a(2)=0, a(3)=6, a(4)=92, a(5)=832, a(6)=4500,
   a(7)=18229, a(8)=58881, a(9)=163509, a(10)=401259,
   a(11)=898420, a(12)=1861146, a(13)=3625546, a(14)=6694982,
   a(15)=11829267, a(16)=20099815. */

#include <stddef.h>

static const seq_t base_vals[17] = {
    0ULL,               /* a(0) unused */
    0ULL, 0ULL,         /* a(1), a(2) */
    6ULL,               /* a(3) */
    92ULL,              /* a(4) */
    832ULL,             /* a(5) */
    4500ULL,            /* a(6) */
    18229ULL,           /* a(7) */
    58881ULL,           /* a(8) */
    163509ULL,          /* a(9) */
    401259ULL,          /* a(10) */
    898420ULL,          /* a(11) */
    1861146ULL,         /* a(12) */
    3625546ULL,         /* a(13) */
    6694982ULL,         /* a(14) */
    11829267ULL,        /* a(15) */
    20099815ULL         /* a(16) */
};

seq_t a(idx_t n)
{
    if (n <= 2)          /* a(1) = a(2) = 0 */
        return 0;
    if (n <= 16)         /* return the stored initial value */
        return base_vals[n];

    /* Buffer holds the last 14 computed values:
       buf[0] = a(i-14), buf[1] = a(i-13), ..., buf[13] = a(i-1) */
    seq_t buf[14];
    for (int j = 0; j < 14; ++j)
        buf[j] = base_vals[j + 3];   /* a(3) … a(16) */

    seq_t val;
    for (idx_t i = 17; i <= n; ++i) {
        val = 4 * buf[13]
            - buf[12]
            - 16 * buf[11]
            + 19 * buf[10]
            + 20 * buf[9]
            - 45 * buf[8]
            + 45 * buf[6]          /* a(i-8) */
            - 20 * buf[5]
            - 19 * buf[4]
            + 16 * buf[3]
            + buf[2]
            - 4 * buf[1]
            + buf[0];
        /* shift the buffer left by one position */
        for (int k = 0; k < 13; ++k)
            buf[k] = buf[k + 1];
        buf[13] = val;   /* buf[13] is now a(i) */
    }
    return buf[13];
}
