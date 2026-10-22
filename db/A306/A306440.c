/*
 * A306440  Number of different ways of expressing 2*n as (p - 1)*(q - 1),
 *          where p < q are primes.
 * https://oeis.org/A306440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways of expressing 2*n as (p-1)*(q-1) with
 *            distinct primes p < q
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* For n = 0, 2*n = 0 cannot be expressed as a product of two
       positive integers (p-1)*(q-1) with primes p, q. */
    if (n == 0) return (seq_t)0;

    idx_t m = n * 2;          /* m = 2*n */
    seq_t count = 0;

    for (idx_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            idx_t e = m / d;          /* complementary divisor */
            if (d < e) {              /* ensure p < q */
                if (is_prime(d + 1) && is_prime(e + 1))
                    count += 1;
            }
        }
    }
    return count;
}
