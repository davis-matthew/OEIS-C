/*
 * A320868  Primes such that p + digitsum(p, base 8) is again a prime.
 * https://oeis.org/A320868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 787, u32 n <= 448, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that p + digit_sum(p, base 8) is also a prime
   Algorithm: sequentially generate primes, compute the base‑8 digit sum
   of each prime, test whether p + sum is prime, and store the qualifying
   primes in a static cache.  The cache is expanded lazily so that any
   call to a(n) obtains the term regardless of order of access. */

/* include required utilities */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t start) */
#include "../utils/is_prime.h"    /* bool is_prime(idx_t x) */
#include "../utils/is_prime_big.h"/* bool is_prime_big(seq_t n) */

/* local helper: allocate or grow a cache array */
static void ensure_cache_cap(size_t *cap, seq_t **arr, size_t needed) {
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        if (new_cap < needed) new_cap = needed;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

seq_t a(idx_t n) {
    /* static cache for already found terms */
    static seq_t *cache = NULL;   /* array of qualifying primes */
    static size_t cache_cap = 0;
    static size_t cache_size = 0; /* number of stored terms */
    static idx_t next_scan = 2;   /* number to start searching for the next prime */

    /* expand the cache until we have at least n terms */
    while (cache_size < n) {
        /* find next prime >= next_scan */
        idx_t prime_idx = next_prime(next_scan);
        next_scan = prime_idx + 1; /* advance for subsequent searches */

        /* prime found */
        seq_t p   = (seq_t)prime_idx;   /* convert to seq_t (may overflow if too large) */
        seq_t sum = (seq_t)0;           /* digit sum in base 8 */

        /* compute the digit sum in base 8 */
        seq_t t = p;
        while (t > (seq_t)0) {
            sum += t % (seq_t)8; /* modulo 8 is allowed on seq_t */
            t   /= (seq_t)8;
        }

        /* test whether p + sum is also prime */
        seq_t q = p + sum;
        if (is_prime_big(q)) {
            /* store the qualifying prime in the cache */
            ensure_cache_cap(&cache_cap, &cache, cache_size + 1);
            cache[cache_size++] = p;
        }
    }

    return cache[n - 1];
}
