/*
 * A266376  G.f. = b(2)*b(4)*b(6)/(x^9+x^8+x^7-2*x^3-x^2-x+1), where b(k) =
 *          (1-x^k)/(1-x).
 * https://oeis.org/A266376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+2*a(n-3)-a(n-7)-a(n-8)-a(n-9) for n>=10
 *            Initial values: a(0)=1, a(1)=4, a(2)=10, a(3)=23, a(4)=49,
 *            a(5)=100, a(6)=202, a(7)=404, a(8)=804, a(9)=1598
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Pre‑computed first ten terms */
    static const seq_t init[10] = {
        (seq_t)1,  (seq_t)4,  (seq_t)10, (seq_t)23,
        (seq_t)49, (seq_t)100, (seq_t)202, (seq_t)404,
        (seq_t)804, (seq_t)1598
    };

    if (n < 10)
        return init[n];

    /* Circular buffer holding the last 10 values */
    seq_t buf[10];
    for (idx_t i = 0; i < 10; ++i)
        buf[i] = init[i];

    for (idx_t i = 10; i <= n; ++i) {
        seq_t v = buf[(i - 1) % 10]          /* a(i-1) */
                + buf[(i - 2) % 10]          /* a(i-2) */
                + (seq_t)2 * buf[(i - 3) % 10]/* 2*a(i-3) */
                - buf[(i - 7) % 10]          /* a(i-7) */
                - buf[(i - 8) % 10]          /* a(i-8) */
                - buf[(i - 9) % 10];         /* a(i-9) */

        buf[i % 10] = v;
    }

    return buf[n % 10];
}
