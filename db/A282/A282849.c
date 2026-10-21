/*
 * A282849  Number of divisors k of n such that (n + k^2)/k is a prime.
 * https://oeis.org/A282849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1739, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* formula: a(n) = number of divisors k of n such that (n + k^2)/k is a prime */

/* The prime test is provided by the utilities library. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t q = n / d;                /* complementary divisor */
            seq_t t = (seq_t)d + (seq_t)q;   /* (n/k + k) for both k = d and k = q */
            if (is_prime_big(t)) {
                count += (d == q) ? 1 : 2;
            }
        }
    }
    return count;
}
