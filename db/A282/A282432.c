/*
 * A282432  Number of primes of the form n - 3^k.
 * https://oeis.org/A282432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{k ≥ 0 | n − 3^k is prime and n − 3^k > 0}
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n <= 1) return 0;

    idx_t pow3 = 1;
    while (pow3 < n) {
        idx_t m = n - pow3;
        if (is_prime(m)) {
            ++count;
        }
        /* prevent overflow on the next multiplication */
        if (pow3 > UINT64_MAX / 3) break;
        pow3 *= 3;
    }
    return count;
}
