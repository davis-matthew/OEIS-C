/*
 * A244214  a(n) = binomial(2*c-1, c-1) (mod c^3), where c is the n-th
 *          composite.
 * https://oeis.org/A244214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*c-1, c-1) (mod c³) where c is the n‑th
 *            composite
 * Range:     u8 n <= 2, u16 n <= 29, u32 n <= 1386, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"

/* compute the exponent of prime p in n! (Legendre's formula) */
static idx_t _exp_fact(idx_t n, idx_t p)
{
    idx_t e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

seq_t a(idx_t n)
{
    /* n must be >= 1. For n == 0 we return 0, but the sequence is 1‑based. */
    if (n == 0)
        return (seq_t)0;

    /* Find the n-th composite number c. */
    idx_t count = 0;
    idx_t c = 0;
    for (idx_t m = 4;; ++m) {
        if (!is_prime(m)) {
            ++count;
            if (count == n) {
                c = m;
                break;
            }
        }
    }

    /* Compute modulo m = c³.  c³ fits in 64 bits for all n used here. */
    uint64_t mmul = (uint64_t)c * (uint64_t)c * (uint64_t)c;   /* c³ */
    uint64_t mod = mmul;   /* modulus for binomial */

    /* Bound for the binomial coefficient: 2c-1. */
    idx_t limit = 2 * c - 1;

    /* Build a list of primes up to limit. */
    size_t primes_cnt = 0;
    uint64_t *primes = NULL;
    build_primes_upto(limit, &primes, &primes_cnt);

    uint64_t result = 1 % mod;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        idx_t exp = _exp_fact(limit, p) - _exp_fact(c - 1, p) - _exp_fact(c, p);
        if (exp == 0)
            continue;

        /* Compute p^exp mod mod using 128‑bit multiplication. */
        uint64_t term = 1 % mod;
        uint64_t base = p % mod;
        idx_t e = exp;
        while (e) {
            if (e & 1) {
                __uint128_t mul = (__uint128_t)term * base % mod;
                term = (uint64_t)mul;
            }
            e >>= 1;
            if (e) {
                __uint128_t mul = (__uint128_t)base * base % mod;
                base = (uint64_t)mul;
            }
        }

        /* Multiply into result mod mod. */
        __uint128_t mul = (__uint128_t)result * term % mod;
        result = (uint64_t)mul;
    }

    /* Clean up. */
    free(primes);
    return (seq_t)result;
}
