/*
 * A384239  The number of primes p <= n such that p^p == p (mod n).
 * https://oeis.org/A384239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p ≤ n with p^p ≡ p (mod n)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Provided util for modular exponentiation (sequence member is unsigned) */
#include "../utils/mod_pow.h"

/* Trial division primality test for uint64_t values */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2u)
        return false;
    if (x == 2u)
        return true;
    if ((x & 1u) == 0u)
        return false;
    for (uint64_t i = 3u; i * i <= x; i += 2u)
        if (x % i == 0u)
            return false;
    return true;
}

/* Count primes p ≤ n such that p^p ≡ p (mod n) */
seq_t a(idx_t n)
{
    /* Count fits in seq_t (at most π(10000)=1229) */
    seq_t count = 0u;
    /* Loop over all integers up to n */
    for (idx_t p = 2u; p <= n; ++p)
    {
        /* Fast primality test */
        if (!is_prime_uint64((uint64_t)p))
            continue;

        /* Compute p^p mod n.  mod_pow expects seq_t arguments. */
        seq_t rem = mod_pow((seq_t)p, (uint64_t)p, (seq_t)n);

        /* Equivalent to testing rem == p % n, but for p ≤ n the
           equality rem == p holds exactly when the condition is met.
           Using p % n keeps the test correct for p == n. */
        seq_t pmod = (seq_t)(p % n);
        if (rem == pmod)
            ++count;
    }
    return count;
}
