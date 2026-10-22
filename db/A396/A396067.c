/*
 * A396067  Product of odd divisors of n that are <= sqrt(n).
 * https://oeis.org/A396067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product of odd divisors d of n with d ≤ sqrt(n)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t d = 1; d <= n / d; d += 2) {     /* iterate over odd divisors only */
        if (n % d == 0) {
            prod *= (seq_t)d;
        }
    }
    return prod;
}
