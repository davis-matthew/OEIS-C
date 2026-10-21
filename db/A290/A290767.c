/*
 * A290767  Primes p such that p^2 +/- p +/- 1 are all nonprimes.
 * https://oeis.org/A290767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime p such that p^2 ± p ± 1 are all composite
 * Range:     u8 none, u16 n <= 14, u32 n <= 17, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    uint64_t p = 2;
    idx_t found = 0;

    while (1) {
        seq_t psq = (seq_t)p * (seq_t)p;
        seq_t v1 = psq + (seq_t)p + 1;
        seq_t v2 = psq + (seq_t)p - 1;
        seq_t v3 = psq - (seq_t)p + 1;
        seq_t v4 = psq - (seq_t)p - 1;

        if (!is_prime_big(v1) &&
            !is_prime_big(v2) &&
            !is_prime_big(v3) &&
            !is_prime_big(v4)) {
            if (++found == n)
                return (seq_t)p;
        }

        p = next_prime(p + 1);
    }
}
