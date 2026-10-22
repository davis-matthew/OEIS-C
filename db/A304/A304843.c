/*
 * A304843  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2, 4
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 8*a(n-3) + a(n-4) - 9*a(n-5) -
 *            26*a(n-6) - 6*a(n-7) + 6*a(n-8) + 16*a(n-9) + 2*a(n-10) +
 *            6*a(n-11) with a(1)=4, a(2)=12, a(3)=25, a(4)=51, a(5)=165,
 *            a(6)=419, a(7)=1009, a(8)=2697, a(9)=7091, a(10)=18147,
 *            a(11)=46901, a(12)=122403. The recurrence holds for n>12
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial terms a(1)..a(12).  Index 0 unused (offset 1). */
    static const seq_t init[12] = {
        /* a(1) */ 4,
        /* a(2) */ 12,
        /* a(3) */ 25,
        /* a(4) */ 51,
        /* a(5) */ 165,
        /* a(6) */ 419,
        /* a(7) */ 1009,
        /* a(8) */ 2697,
        /* a(9) */ 7091,
        /* a(10)*/ 18147,
        /* a(11)*/ 46901,
        /* a(12)*/ 122403
    };

    if (n <= 12)
        return init[n-1];

    /* Buffer for the last 12 values: buf[0] = a(k-12), ..., buf[11] = a(k-1)  */
    seq_t buf[12];
    for (int i = 0; i < 12; ++i)
        buf[i] = init[i];

    for (idx_t k = 13; k <= n; ++k)
    {
        /* Compute a(k) from the buffered previous 11 values. */
        seq_t sum = buf[11]                         /* a(k-1) */
                 + 2 * buf[10]                      /* a(k-2) */
                 + 8 * buf[9]                       /* a(k-3) */
                 + buf[8]                          /* a(k-4) */
                 + 6 * buf[4]                      /* a(k-8) */
                 + 16 * buf[3]                     /* a(k-9) */
                 + 2 * buf[2]                      /* a(k-10) */
                 + 6 * buf[1];                     /* a(k-11) */

        seq_t sub = 9 * buf[7]                      /* a(k-5) */
                 + 26 * buf[6]                     /* a(k-6) */
                 + 6 * buf[5];                     /* a(k-7) */

        seq_t val = sum - sub;

        /* Shift buffer to make room for new value. */
        for (int i = 0; i < 11; ++i)
            buf[i] = buf[i+1];
        buf[11] = val;
    }

    return buf[11];
}
