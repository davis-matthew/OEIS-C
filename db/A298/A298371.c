/*
 * A298371  a(n) = Sum_{m=0..n} Sum_{i=0..m} i*C(m-i,i)*C(m-i,n-m-i).
 * https://oeis.org/A298371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - a(n-4) - 4*a(n-5) - 3*a(n-6) -
 *            2*a(n-7) - a(n-8) (n>7) a(0)=0, a(1)=0, a(2)=1, a(3)=3, a(4)=7,
 *            a(5)=17, a(6)=40, a(7)=90
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 31, u64 n <= 64, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;
    static size_t sz = 0;

    if (n >= sz) {
        size_t new_sz = n + 1;                /* size needed to store index n */
        seq_t *newbuf = (seq_t *)realloc(buf, new_sz * sizeof(seq_t));
        if (!newbuf) return 0;                /* memory error: return 0 as fallback */
        buf = newbuf;
        for (size_t i = sz; i <= n; ++i) {
            if (i == 0) buf[i] = 0;
            else if (i == 1) buf[i] = 0;
            else if (i == 2) buf[i] = 1;
            else if (i == 3) buf[i] = 3;
            else if (i == 4) buf[i] = 7;
            else if (i == 5) buf[i] = 17;
            else if (i == 6) buf[i] = 40;
            else if (i == 7) buf[i] = 90;
            else {
                /* recurrence: a(n) = 2*a(n-1) + a(n-2)
                                 - a(n-4) - 4*a(n-5) - 3*a(n-6) - 2*a(n-7) - a(n-8) */
                seq_t sum = 2 * buf[i - 1] + buf[i - 2];
                sum -= buf[i - 4];
                sum -= 4 * buf[i - 5];
                sum -= 3 * buf[i - 6];
                sum -= 2 * buf[i - 7];
                sum -= buf[i - 8];
                buf[i] = sum;
            }
        }
        sz = new_sz;
    }
    return buf[n];
}
