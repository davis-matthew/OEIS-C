/*
 * A398199  Upper (3/5,1) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A398199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-5) - 2*a(n-6) + a(n-7) Initial
 *            values: a(0)=0, a(1)=2, a(2)=6, a(3)=13, a(4)=22, a(5)=34,
 *            a(6)=49
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t a(idx_t n) {
    /* handle the trivial cases directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)13;
    if (n == 4) return (seq_t)22;
    if (n == 5) return (seq_t)34;
    if (n == 6) return (seq_t)49;

    /* a circular buffer storing the last seven values */
    seq_t buf[7];
    buf[0] = (seq_t)0;
    buf[1] = (seq_t)2;
    buf[2] = (seq_t)6;
    buf[3] = (seq_t)13;
    buf[4] = (seq_t)22;
    buf[5] = (seq_t)34;
    buf[6] = (seq_t)49;

    for (idx_t i = 7; i <= n; ++i) {
        /* indices modulo 7 for the last seven values */
        idx_t m1 = (i - 1) % 7;
        idx_t m2 = (i - 2) % 7;
        idx_t m5 = (i - 5) % 7;
        idx_t m6 = (i - 6) % 7;
        idx_t m7 = (i - 7) % 7;

        /* compute a(i) using the recurrence */
        seq_t val = 2 * buf[m1];
        val = val + buf[m5];
        val = val + buf[m7];
        val = val - buf[m2];
        val = val - (2 * buf[m6]);

        buf[i % 7] = val;
    }

    return buf[n % 7];
}
