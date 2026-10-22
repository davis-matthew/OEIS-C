/*
 * A309120  a(n) is the least k > 1 such that n*k is adjacent to a prime.
 * https://oeis.org/A309120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > 1 such that n*k ± 1 is prime (adjacent to a
 *            prime)
 * Range:     u8 n <= 46, u16 n <= 5817, u32 n <= 6066, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Return the least k > 1 for which n*k-1 or n*k+1 is prime. */
seq_t a(idx_t n)
{
    /* n is 1-indexed */
    for (idx_t k = 2;; ++k)
    {
        /* For odd n, k must be even – otherwise n*k±1 is even. */
        if ((n & 1) && (k & 1))
            continue;

        seq_t prod = (seq_t)n * (seq_t)k;
        if (is_prime_big(prod - 1) || is_prime_big(prod + 1))
            return (seq_t)k;
    }
}
