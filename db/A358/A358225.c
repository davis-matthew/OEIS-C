/*
 * A358225  Numbers k such that A276086(k) mod k is an odd number, where
 *          A276086 is the primorial base exp-function.
 * https://oeis.org/A358225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer k such that A276086(k) mod k is
 *            odd, where A276086(k) = Π p_i^{d_i} with k expressed in
 *            primorial base (mixed–radix) using primes
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"

/* Compute (base^exp) mod mod with 64‑bit arithmetic */
static uint64_t powmod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1)
            result = (uint64_t)(((__uint128_t)result * b) % mod);
        b = (uint64_t)(((__uint128_t)b * b) % mod);
        exp >>= 1;
    }
    return result;
}

/* Return true iff k satisfies A276086(k) mod k is odd */
static bool parity_ok(idx_t k)
{
    if (k <= 1) return false;           /* k starts at 2 in the sequence */

    uint64_t rem = 1 % (uint64_t)k;     /* running product modulo k */
    uint64_t temp = (uint64_t)k;        /* remaining part of k to decompose */
    idx_t i = 0;                        /* digit index (prime_1 = 2) */

    while (temp > 0) {
        uint64_t prime = (uint64_t)get_prime(i + 1);
        uint64_t d = temp % prime;      /* Cantor digit */

        uint64_t powv = powmod_uint64(prime, d, k);
        rem = (uint64_t)(((__uint128_t)rem * powv) % k);

        temp /= prime;
        ++i;
    }

    return (rem & 1U) == 1U;            /* remainder odd? */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t count = 0;            /* number of qualifying numbers found */
    idx_t candidate = 2;        /* start from 2 */

    while (1) {
        if (parity_ok(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
