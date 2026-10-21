/*
 * A293644  a(n) is the integer k that minimizes |k/Fibonacci(n) - 3/5|.
 * https://oeis.org/A293644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+2*a(n-2)-a(n-3)-2*a(n-4)+a(n-5)+2*a(n-6) *
 *            -a(n-7)-2*a(n-8)+a(n-9)+a(n-10), for n >= 11. * Initial values:
 *            a(0)=0, a(1)=1, a(2)=1, a(3)=1, a(4)=2, a(5)=3, * a(6)=5,
 *            a(7)=8, a(8)=13, a(9)=20, a(10)=33
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 48, u64 n <= 94, u128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t init[11] = {0, 1, 1, 1, 2, 3, 5, 8, 13, 20, 33};

    if (n < 11)
        return init[(size_t)n];

    seq_t buf[10];
    /* buf[0] = a(1), buf[1] = a(2), ..., buf[9] = a(10) */
    for (idx_t i = 1; i <= 10; ++i)
        buf[(size_t)(i - 1)] = init[(size_t)i];

    for (idx_t i = 11; i <= n; ++i) {
        seq_t next = buf[9]
                    + 2 * buf[8]
                    - buf[7]
                    - 2 * buf[6]
                    + buf[5]
                    + 2 * buf[4]
                    - buf[3]
                    - 2 * buf[2]
                    + buf[1]
                    + buf[0];

        /* shift the sliding window left by one */
        for (size_t j = 0; j < 9; ++j)
            buf[j] = buf[j + 1];
        buf[9] = next;
    }
    return buf[9];
}
