/*
 * A253469  Number of (n+1) X (7+1) 0..2 arrays with every 2 X 2 subblock
 *          diagonal maximum minus antidiagonal minimum unequal to its
 *          neighbors horizontally, vertically, diagonally and antidiagonally.
 * https://oeis.org/A253469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-2) - 4*a(n-4) for n > 6, with initial values
 *            a(1)=7227096, a(2)=44464, a(3)=17000, a(4)=15680, a(5)=20000,
 *            a(6)=20864
 * Range:     u8 none, u16 none, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    static const seq_t init[7] = {0, 7227096, 44464, 17000, 15680, 20000, 20864};

    seq_t buf[5];          /* sliding window of last 5 terms */

    for (idx_t i = 1; i <= n; ++i) {
        if (i <= 6) {
            buf[i % 5] = init[i];
        } else {
            buf[i % 5] = 5 * buf[(i - 2) % 5] - 4 * buf[(i - 4) % 5];
        }
    }

    return buf[n % 5];
}
