/*
 * A322978  Number of even divisors d of 2n such that d-1 is prime.
 * https://oeis.org/A322978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of even divisors d of 2n such that d‑1 is
 *            prime
 * Range:     u8 n <= 15120, u16 n <= 15120, u32 n <= 15120, u64 n <= 15120,
 *            u128 n <= 15120, big any n
 * Validated: standard, pass, 121 terms through n = 8393 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The even divisors of 2n are 2t for the divisors t of n, so a(n) counts the
   t | n with 2t - 1 prime. Primality by the exact 64-bit test (the original
   used a sieve to 20000, enough only for n <= 10000). */

#include <stdint.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            if (is_prime_u64(2 * (uint64_t)i - 1)) count++;
            idx_t t2 = n / i;
            if (t2 != i && is_prime_u64(2 * (uint64_t)t2 - 1)) count++;
        }
    }
    return count;
}
