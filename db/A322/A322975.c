/*
 * A322975  Number of divisors d of n such that d-2 is prime.
 * https://oeis.org/A322975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that d-2 is prime
 * Range:     u8 n <= 9760, u16 n <= 9760, u32 n <= 9760, u64 n <= 9760,
 *            u128 n <= 9760, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;
            if (d1 > 2 && is_prime(d1 - 2)) ++count;
            if (d2 != d1 && d2 > 2 && is_prime(d2 - 2)) ++count;
        }
    }
    return count;
}
