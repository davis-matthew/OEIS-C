/*
 * A252221  Number of (n+2) X (1+2) 0..2 arrays with every 3 X 3 subblock row
 *          and column sum 2 3 or 4 and every diagonal and antidiagonal sum
 *          not 2 3 or 4.
 * https://oeis.org/A252221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) + 3*a(n-4) - 2*a(n-5) -
 *            2*a(n-7) for n>10, with initial values a(1)=228, a(2)=368,
 *            a(3)=812, a(4)=1442, a(5)=2458, a(6)=4738, a(7)=9922,
 *            a(8)=20070, a(9)=38774, a(10)=75106 This recurrence reproduces
 *            the OEIS sequence A252221
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 26, u64 n <= 58, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base values for n=1..10 */
    static const seq_t base[10] = {
        228, 368, 812, 1442, 2458,
        4738, 9922, 20070, 38774, 75106
    };

    if (n <= 10)
        return base[n - 1];

    /* Buffer holds a(i-7)..a(i-1) for current i.
       Initially i=11, so load a(4)..a(10). */
    seq_t buf[7];
    for (int i = 0; i < 7; ++i)
        buf[i] = base[3 + i];          /* base[3] = a(4) */

    idx_t i = 11;
    seq_t cur = 0;

    for (; i <= n; ++i) {
        /* Compute a(i) using the recurrence:
           a = 2*buf[6] - buf[5] + buf[4] + 3*buf[3] - 2*buf[2] - 2*buf[0] */
        seq_t sum1 = 2 * buf[6] + buf[4] + 3 * buf[3];
        seq_t sum2 = buf[5] + 2 * buf[2] + 2 * buf[0];
        cur = sum1 - sum2;

        /* Shift buffer left by one, insert new value at buf[6] */
        for (int k = 0; k < 6; ++k)
            buf[k] = buf[k + 1];
        buf[6] = cur;

        if (i == n)
            return cur;
    }

    return 0;  /* should never reach here */
}
