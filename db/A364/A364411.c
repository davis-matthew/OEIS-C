/*
 * A364411  a(n) = prime(n) + 2*prime(n+1).
 * https://oeis.org/A364411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) + 2*prime(n+1)
 * Range:     u8 n <= 22, u16 n <= 2449, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* 1‑based cache of primes, updated on demand */
    static uint64_t *primes = NULL;
    static size_t primes_cap = 0;    /* capacity of the cache */
    static size_t primes_used = 0;   /* number of primes stored */

    /* We need primes up to index n+1 (prime(n+1)). */
    if (primes_used < (size_t)(n + 1)) {
        /* Lazily allocate the cache. */
        if (primes_used == 0) {
            primes_cap = 16;
            primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
            if (!primes) return (seq_t)0;   /* out of memory – unlikely in test harness */
        }

        /* Start searching from the next integer after the last prime. */
        uint64_t candidate = (primes_used == 0) ? 2 : primes[primes_used - 1] + 1;

        while (primes_used < (size_t)(n + 1)) {
            if (is_prime((idx_t)candidate)) {
                if (primes_used == primes_cap) {
                    primes_cap <<= 1;
                    uint64_t *tmp = (uint64_t *)realloc(primes,
                               primes_cap * sizeof(uint64_t));
                    if (!tmp) return (seq_t)0;   /* out of memory */
                    primes = tmp;
                }
                primes[primes_used++] = candidate;
            }
            ++candidate;
        }
    }

    seq_t p_n   = (seq_t)primes[n - 1];
    seq_t p_np1 = (seq_t)primes[n];
    return p_n + ((seq_t)2 * p_np1);
}
