/*
 * A291615  Number of primes p < prime(n) such that p is a primitive root
 *          modulo prime(n) and also a primitive root modulo prime(p).
 * https://oeis.org/A291615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1006, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   factor_u64, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A291615
   a(n) = number of primes p < prime(n) such that p is a primitive root
          modulo prime(n) and also a primitive root modulo prime(p).
   Offset 1 (a(1)=0). This code uses the utilities get_prime, factor_u64
   and mod_pow available in the build. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"

/* Test whether `g` is a primitive root modulo the prime `p`. */
static bool is_primitive_root(seq_t g, seq_t p)
{
    /* p is prime, so ord = p-1. */
    seq_t ord = p - 1;
    uint64_t ord64 = (uint64_t)ord;   /* ord <= 1e7 for n <= 1e4 */

    /* Factor ord into distinct prime factors. */
    uint64_t prime_factors[64];
    uint64_t exponents[64];
    size_t num_factors = 0;
    factor_u64(ord64, prime_factors, exponents, &num_factors);

    for (size_t i = 0; i < num_factors; ++i) {
        uint64_t q = prime_factors[i];
        uint64_t exp = ord64 / q;          /* exponent for the test */
        if (mod_pow(g, exp, p) == 1) /* g^((p-1)/q) ≡ 1 (mod p) */
            return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* safety for out‑of‑bounds indices */

    /* Prime at position n (1‑based). */
    seq_t prime_n = (seq_t)get_prime(n);

    seq_t count = 0;
    idx_t i = 1;                   /* start from the first prime (2) */
    while (1) {
        seq_t p = (seq_t)get_prime(i);
        if (p >= prime_n) break;   /* only primes < prime(n) */

        if (is_primitive_root(p, prime_n)) {
            /* prime(p) is the p‑th prime (p itself is a prime). */
            seq_t prime_of_p = (seq_t)get_prime((idx_t)p);
            if (is_primitive_root(p, prime_of_p))
                ++count;
        }
        ++i;
    }
    return count;
}
