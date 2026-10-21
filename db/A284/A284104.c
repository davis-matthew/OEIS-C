/*
 * A284104  a(n) = Sum_{d|n, d == 5 (mod 6)} d.
 * https://oeis.org/A284104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d == 5 (mod 6)} d
 * Range:     u8 n <= 244, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;                /* complementary divisor */
            if (d % 6 == 5) {
                sum += (seq_t)d;
            }
            if (e != d && e % 6 == 5) {
                sum += (seq_t)e;
            }
        }
    }
    return sum;
}
