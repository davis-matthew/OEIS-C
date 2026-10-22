/*
 * A373198  Number of squarefree numbers from prime(n) to prime(n+1) - 1.
 * https://oeis.org/A373198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of squarefree numbers from prime(n) to
 *            prime(n+1)-1
 * Range:     u8 n <= 53, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   is_squarefree, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a cached first‑n prime sieve and a simple loop over the interval. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_squarefree.h"   /* declares bool is_squarefree(seq_t) */
#include "../utils/sieve_first_n_primes.h"  /* declares void sieve_first_n_primes(idx_t, uint32_t **, size_t *) */

static uint32_t *prime_cache = NULL;   /* NULL until primes are computed */
static size_t prime_cache_count = 0;   /* number of primes stored in prime_cache */

/* Ensure we have the first 'needed' primes cached. */
static void ensure_prime_cache(idx_t needed) {
    if (prime_cache_count >= needed) return;
    if (prime_cache) free(prime_cache);
    prime_cache = NULL;
    sieve_first_n_primes(needed, &prime_cache, &prime_cache_count);
}

/* Return a(n).  For n < 1 (outside the offset) the function returns 0. */
seq_t a(idx_t n) {
    if (n < 1) return 0;                /* values below the offset are irrelevant */
    idx_t needed = n + 1;                /* we need prime(n) and prime(n+1) */
    ensure_prime_cache(needed);

    seq_t count = 0;
    idx_t start = (idx_t)prime_cache[n - 1];   /* prime(n) */
    idx_t end   = (idx_t)prime_cache[n];       /* prime(n+1) */

    for (idx_t val = start; val < end; ++val) {
        if (is_squarefree((seq_t)val))
            ++count;
    }
    return count;
}
