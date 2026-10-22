/*
 * A399374  For n >= 2, the least significant nonzero digit in primorial base
 *          representation of n' (the arithmetic derivative of n), with a(0) =
 *          a(1) = 0 by convention.
 * https://oeis.org/A399374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = least significant non‑zero digit in the primorial base *
 *            representation of the arithmetic derivative of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = a(1) = 0 by definition. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the derivative is 0, so a(n) = 0. */
    if (n <= 1) return (seq_t)0;

    /* Factorise n.  According to the framework the number of
     * distinct primes for a 64‑bit integer is less than 64, so this
     * buffer is sufficient. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    uint64_t p = 2;                 /* first prime in the primorial base   */
    unsigned long long w = 1;        /* product of all previous primes      */

    while (true) {
        /* The modulus for the current digit is w * p.  For the test range
         * (n up to about 10 000) this value comfortably fits inside
         * 'unsigned long long'. */
        unsigned long long modulus = w * p;

        /* Compute deriv mod modulus.  Each term is
         * e_i * (n / prime_i).  The sum is performed modulo 'modulus'. */
        unsigned long long deriv_mod = 0;
        for (size_t i = 0; i < cnt; ++i) {
            unsigned long long term = (n / primes[i]) * exps[i];
            deriv_mod = (deriv_mod + term) % modulus;
        }

        /* The digit at this position is (deriv / w) mod p.  Since
         * all arithmetic is performed in unsigned long long,
         * the division and modulo are exact. */
        unsigned long long digit = (deriv_mod / w) % p;
        if (digit != 0) return (seq_t)digit;

        /* Advance to the next base prime. */
        w *= p;
        p = next_prime(p);
    }

    /* Unreachable, but keeps the compiler happy. */
    return (seq_t)0;
}
