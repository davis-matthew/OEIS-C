/*
 * A279051  Sum of odd nonprime divisors of n.
 * https://oeis.org/A279051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d odd nonprime} d
 * Range:     u8 n <= 164, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d2 = n / d;
            /* handle divisor d */
            if ((d & 1) && !(d > 1 && is_prime(d))) {
                sum += (seq_t)d;
            }
            /* handle paired divisor d2, avoid double counting if d == d2 */
            if (d2 != d && (d2 & 1) && !(d2 > 1 && is_prime(d2))) {
                sum += (seq_t)d2;
            }
        }
    }
    return sum;
}
