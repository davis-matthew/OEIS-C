/*
 * A335940  a(n) = n if n is prime, a(n) = 0 if n is a nontrivial power of a
 *          prime, and otherwise a(n) = max{|p-q| where p, q are distinct
 *          primes dividing n}.
 * https://oeis.org/A335940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n if n is prime;
 *           a(n) = 0 if n is a nontrivial power of a prime;
 *           otherwise a(n) = max{|p – q| : p, q distinct prime divisors of n}.
 * This implementation uses the provided utility functions:
 *   is_prime(idx_t n)
 *   factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition in OEIS */
    if (n == 1) {
        return (seq_t)1;
    }

    /* If n is prime, return n itself */
    if (is_prime(n)) {
        return (seq_t)n;
    }

    /* Factor n to obtain its prime divisors and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* n is a prime power (single prime factor, exponent > 1) */
    if (cnt == 1 && exps[0] >= 2) {
        return (seq_t)0;
    }

    /* For composites with at least two distinct primes,
       compute the maximum prime difference. */
    seq_t maxdiff = 0;
    for (size_t i = 0; i < cnt; ++i) {
        for (size_t j = i + 1; j < cnt; ++j) {
            uint64_t diff64 = (primes[i] > primes[j]) ?
                               (primes[i] - primes[j]) :
                               (primes[j] - primes[i]);
            seq_t diff = (seq_t)diff64;
            if (diff > maxdiff) {
                maxdiff = diff;
            }
        }
    }
    return maxdiff;
}
