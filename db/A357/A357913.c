/*
 * A357913  Inverse of 10 modulo prime(n).
 * https://oeis.org/A357913
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = powmod(10, prime(n)-2, prime(n)) (10^(p-2) mod p)
 * Range:     u8 n <= 54, u16 n <= 6667, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the modular inverse of 10 modulo the n-th prime. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Prime retrieval memoization */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t  prime_limit = 0;

    /* Ensure we have at least n primes in the cache */
    if (n > prime_cnt) {
        /* Rough over‑estimate of the n-th prime (works comfortably up to >10⁵). */
        idx_t need_limit = n * 20 + 100;
        if (!primes || prime_limit < need_limit) {
            free(primes);          /* free previous cache if any */
            build_primes_upto(need_limit, &primes, &prime_cnt);
            prime_limit = need_limit;
        }
    }

    if (n == 0)      /* undefined, but return 0 to keep the function total */
        return 0;

    /* The (1‑based) n-th prime */
    seq_t p = (seq_t)primes[n - 1];

    /* Compute 10^(p-2) mod p */
    return mod_pow((seq_t)10, (uint64_t)(p - 2), p);
}
