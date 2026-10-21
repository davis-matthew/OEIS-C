/*
 * A293513  Number of proper divisors of n of the form 4k+3.
 * https://oeis.org/A293513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d<n} [3 == d mod 4] (number of proper divisors
 *            of n of the form 4k+3)
 * Range:     u8 n <= 8540, u16 n <= 8540, u32 n <= 8540, u64 n <= 8540,
 *            u128 n <= 8540, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    /* Iterate through all divisors up to sqrt(n) without overflow */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* Check divisor i */
            if (i != n && (i & 3ULL) == 3U) {
                ++count;
            }
            /* Check the paired divisor j = n / i */
            idx_t j = n / i;
            if (j != i && j != n && (j & 3ULL) == 3U) {
                ++count;
            }
        }
    }
    return count;
}
