/*
 * A284211  a(n) is the least positive integer such that n^2 + a(n)^2 and n^2
 *          + (a(n) + 2)^2 are primes.
 * https://oeis.org/A284211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 583, u32 n <= 72, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a284211: a(n) is the least positive integer k such that
 * n^2 + k^2 and n^2 + (k+2)^2 are both prime.
 * Computed by brute–force search for k >= 1. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n is at least 1; the offset starts at 1. */
    seq_t nn = (seq_t)n * (seq_t)n;

    for (seq_t k = 1;; ++k) {
        seq_t val1 = nn + k * k;
        seq_t two_k = k + 2;
        seq_t val2 = nn + two_k * two_k;
        if (is_prime_big(val1) && is_prime_big(val2))
            return k;
    }
}
