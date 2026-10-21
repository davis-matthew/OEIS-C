/*
 * A284443  a(n) = Sum_{d|n, d == 2 (mod 7)} d.
 * https://oeis.org/A284443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d ≡ 2 (mod 7)} d
 * Range:     u8 n <= 239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* iterate over divisor pairs up to sqrt(n) avoiding overflow
       by using integer division to control the upper bound. */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;
            if (d1 % 7 == 2u) {
                sum += (seq_t)d1;
            }
            if (d2 != d1 && d2 % 7 == 2u) {
                sum += (seq_t)d2;
            }
        }
    }
    return sum;
}
