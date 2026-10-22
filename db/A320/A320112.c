/*
 * A320112  a(n) = Product_{d|n, d>1} prime(1+(d mod 12)).
 * https://oeis.org/A320112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n, d>1} prime(1+(d mod 12))
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 59, u64 n <= 359,
 *            u128 n <= 2519, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* primes[0] = prime(1) = 2, primes[1] = prime(2) = 3, ... primes[11] = prime(12) = 37 */
    static const seq_t primes12[12] = {2,3,5,7,11,13,17,19,23,29,31,37};

    if (n == 1) {
        return 1;               /* no divisor >1 */
    }

    seq_t result = 1;
    /* iterate over proper divisors d > 1 except n itself */
    for (idx_t d = 2; d <= n / d; ++d) {
        if (n % d == 0) {
            result *= primes12[d % 12];
            idx_t other = n / d;
            if (other != d && other > 1) {
                result *= primes12[other % 12];
            }
        }
    }

    /* include the divisor n itself */
    result *= primes12[n % 12];
    return result;
}
