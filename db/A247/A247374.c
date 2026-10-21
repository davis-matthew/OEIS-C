/*
 * A247374  Number of button presses required to try every combination of a
 *          binary combination lock with n number buttons.
 * https://oeis.org/A247374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 25, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A247374
 *  Recurrence:
 *      a(1)=3, a(2)=8, a(3)=17, a(4)=38, a(5)=77
 *      for n≥6:
 *          a(n) = ((4*n-2)*a(n-1) - (n-2)*a(n-2)
 *                - (14*n-20)*a(n-3) + (20*n-44)*a(n-4)
 *                - (8*n-24)*a(n-5)) / n
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n = 0, not used */

    /* Static buffers for memoisation */
    static seq_t *buf = NULL;      /* buffer holding a[1..max] */
    static idx_t last = 0;         /* highest index already computed */
    static size_t cap = 0;         /* allocated capacity of buf */

    /* Extend buffer if required */
    if (n > last) {
        size_t newcap = (size_t)n + 1;   /* need indices 0..n */
        if (newcap > cap) {
            buf = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
            cap = newcap;
        }

        /* Initialise known values and compute missing terms */
        idx_t i;
        for (i = last + 1; i <= n; ++i) {
            if (i == 1)         buf[1] = (seq_t)3;
            else if (i == 2)    buf[2] = (seq_t)8;
            else if (i == 3)    buf[3] = (seq_t)17;
            else if (i == 4)    buf[4] = (seq_t)38;
            else if (i == 5)    buf[5] = (seq_t)77;
            else { /* n ≥ 6 */
                /* Coefficients (as unsigned values for products) */
                idx_t c1 = 4 * i - 2;      /* 4*n-2   */
                idx_t c2 = i - 2;          /* n-2     */
                idx_t c3 = 14 * i - 20;    /* 14*n-20 */
                idx_t c4 = 20 * i - 44;    /* 20*n-44 */
                idx_t c5 = 8 * i - 24;     /* 8*n-24  */

                /* Sum of positive terms: (4*n-2)*a(n-1) + (20*n-44)*a(n-4) */
                seq_t sum_pos = 0;
                sum_pos += ((seq_t)c1) * buf[i-1];
                sum_pos += ((seq_t)c4) * buf[i-4];

                /* Sum of negative terms: (n-2)*a(n-2) + (14*n-20)*a(n-3) + (8*n-24)*a(n-5) */
                seq_t sum_neg = 0;
                sum_neg += ((seq_t)c2) * buf[i-2];
                sum_neg += ((seq_t)c3) * buf[i-3];
                sum_neg += ((seq_t)c5) * buf[i-5];

                /* a(n) = (sum_pos - sum_neg) / n */
                buf[i] = (sum_pos - sum_neg) / ((seq_t)i);
            }
        }
        last = n;   /* preserve highest computed index */
    }

    return buf[n];
}
