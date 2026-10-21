/*
 * A257586  Numbers that are neither primes nor three times primes.
 * https://oeis.org/A257586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer that is NOT a prime and NOT
 *            3×prime
 * Range:     u8 n <= 178, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimal helper: deterministic primality test for small uint64_t values. */
#include <stdbool.h>
#include <stdint.h>

/* Determines if x is a prime number (works for x up to 2^64). */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        if (k == 1) {
            /* 1 is neither prime nor 3×prime. */
            if (++count == n) return (seq_t)k;
        } else {
            if (is_prime_uint64((uint64_t)k)) continue;          /* skip primes */
            if (k % 3 == 0 && is_prime_uint64((uint64_t)(k / 3))) continue; /* skip 3×prime */
            if (++count == n) return (seq_t)k;
        }
    }
}
