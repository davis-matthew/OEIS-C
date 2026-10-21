/*
 * A280855  Number of nX4 0..2 arrays with no element equal to more than one
 *          of its king-move neighbors and with new values introduced in order
 *          0 sequentially upwards.
 * https://oeis.org/A280855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1)+a(n-2)+a(n-3)+2*a(n-4)+3*a(n-5)
 *              -4*a(n-6)-3*a(n-7)-4*a(n-10)+2*a(n-12)  for n>16
 * Initial values:
 * a(1)=11, a(2)=110, a(3)=96, a(4)=124, a(5)=216, a(6)=462,
 * a(7)=1005, a(8)=2010, a(9)=3907, a(10)=7756, a(11)=15749,
 * a(12)=31804, a(13)=63463, a(14)=126758, a(15)=254453, a(16)=511180
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of range, not used */

    /* base values a(1)..a(16) */
    static const seq_t base[16] = {
        11,      110,      96,      124,      216,
        462,     1005,     2010,    3907,     7756,
        15749,   31804,    63463,   126758,  254453,
        511180
    };

    if (n <= 16) return base[n-1];

    /* Maintain a circular buffer of the last 12 values:
     * prev[0] = a(i-12), prev[11] = a(i-1) */
    seq_t prev[12];
    for (idx_t j = 0; j < 12; ++j)
        prev[j] = base[4 + j];           /* a(5)..a(16) */

    seq_t val = 0;
    for (idx_t i = 17; i <= n; ++i)
    {
        /* recurrence applied using the buffer indices */
        val =  prev[11]                             /* a(i-1) */
             + prev[10]                             /* a(i-2) */
             + prev[9]                              /* a(i-3) */
             + prev[8] * (seq_t)2                   /* 2*a(i-4) */
             + prev[7] * (seq_t)3                   /* 3*a(i-5) */
             - prev[6] * (seq_t)4                   /* -4*a(i-6) */
             - prev[5] * (seq_t)3                   /* -3*a(i-7) */
             - prev[2] * (seq_t)4                   /* -4*a(i-10) */
             + prev[0] * (seq_t)2;                  /* +2*a(i-12) */

        /* shift buffer left and append new value */
        for (idx_t j = 0; j < 11; ++j)
            prev[j] = prev[j+1];
        prev[11] = val;
    }

    return val;
}
