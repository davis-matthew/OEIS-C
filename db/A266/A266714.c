/*
 * A266714  Number of k <= n such that (n mod k) is prime.
 * https://oeis.org/A266714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of k <= n such that (n mod k) is a prime
 * Range:     u8 n <= 766, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t rem = n % k;
        if (rem >= 2 && is_prime(rem))
            ++count;
    }
    return count;
}
