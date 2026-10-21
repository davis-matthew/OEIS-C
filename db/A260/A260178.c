/*
 * A260178  a(n) = hyperfactorial(prime(n)-1) mod prime(n).
 * https://oeis.org/A260178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Π_{i=1}^{prime(n)-1} i^i mod prime(n)
 * Range:     u8 n <= 54, u16 n <= 5440, u32 n <= 6748, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* small modular exponentiation: base^exp (mod mod)  */
static seq_t mod_pow_uint(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1U)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1U;
    }
    return result;
}

/* static cache of the sequence primes */
static bool primes_initialized = false;
static idx_t *primes_cache = NULL;
static size_t primes_cached = 0;

/* Returns the nth term of A260178 – 1‑based indexing */
seq_t a(idx_t n)
{
    /* initialise prime cache once */
    if (!primes_initialized) {
        /* 10000th prime ≈ 104729; 200000 is safely above it */
        const idx_t limit = 200000U;
        build_primes_upto(limit, &primes_cache, &primes_cached);
        primes_initialized = true;
    }

    /* n is 1‑based index – guard against out of bounds */
    if (n == 0 || n > (idx_t)primes_cached)
        return 0;

    /* prime(p) */
    uint32_t p_32 = (uint32_t)primes_cache[(size_t)(n - 1)];
    seq_t p = (seq_t)p_32;          /* cast to seq_t */
    seq_t prod = 1;                 /* running product modulo p */

    for (idx_t i = 1; i < p_32; ++i) {
        seq_t term = mod_pow_uint((seq_t)i, i, p);
        prod = (prod * term) % p;
    }

    return prod;
}
