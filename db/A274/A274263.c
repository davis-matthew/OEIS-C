/*
 * A274263  Integer part of the ratio of consecutive prime gaps.
 * https://oeis.org/A274263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(A001223(n+1)/A001223(n)) – integer part of the
 *            ratio of consecutive prime gaps
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static uint64_t *prime_cache = NULL;   /* cached primes (uint64_t)   */
static size_t   prime_cap   = 0;        /* capacity of prime_cache    */
static size_t   prime_len   = 0;        /* number of primes stored    */
static uint64_t next_candidate = 2;     /* next number to test for prime */

/* Ensure that `prime_cache` contains at least `need` primes
   (prime indices 0 .. need-1).  The function is safe to call
   repeatedly with increasing or arbitrary `need`. */
static void
ensure_primes(size_t need)
{
    /* Allocate initial storage if not yet done. */
    if (prime_cache == NULL) {
        prime_cap = 128;
        prime_cache = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_cache) abort();
    }

    while (prime_len < need) {
        /* Find next prime. */
        while (!is_prime(next_candidate))
            ++next_candidate;

        /* Store it. */
        prime_cache[prime_len++] = next_candidate;
        ++next_candidate;

        /* Grow the array when necessary. */
        if (prime_len >= prime_cap) {
            prime_cap *= 2;
            void *tmp = realloc(prime_cache, prime_cap * sizeof(uint64_t));
            if (!tmp) abort();
            prime_cache = (uint64_t *)tmp;
        }
    }
}

/* Compute the n-th term of sequence A274263.
   For n < 1 (undefined in OEIS) the function returns 0. */
seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* We need primes up to indices n+1 (0‑based). */
    ensure_primes((size_t)(n + 2));

    const uint64_t g_prev = prime_cache[(size_t)n]   - prime_cache[(size_t)n - 1];
    const uint64_t g_next = prime_cache[(size_t)n + 1] - prime_cache[(size_t)n];

    /* Integer division gives the floor of the ratio. */
    return (seq_t)(g_next / g_prev);
}
