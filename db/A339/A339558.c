/*
 * A339558  Number of divisors of 2n that are the average of a pair of twin
 *          primes.
 * https://oeis.org/A339558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = # of divisors d of 2*n such that d-1 and d+1 are both
 *            primes (i.e., d is the average of a pair of twin primes)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"


seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    uint64_t m = (uint64_t)n << 1;          /* m = 2*n */
    uint64_t count = 0;

    for (uint64_t d = 1; d <= m; ++d) {
        if (m % d == 0) {
            if (is_prime((idx_t)(d - 1)) && is_prime((idx_t)(d + 1)))
                ++count;
        }
    }

    return (seq_t)count;
}
