/*
 * A379715  The second Jordan totient function applied to the squarefree
 *          numbers.
 * https://oeis.org/A379715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product_{p|m} (p^2-1) where m is the nth squarefree
 *            number
 * Range:     u8 n <= 11, u16 n <= 157, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* External helper: prime sieve up to a limit. */
#include "../utils/build_primes_upto.h"

/* Static state for memoisation and prime data. */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;      /* Allocated cache size */
static idx_t cached_max = 0;     /* Largest n computed so far */

static uint64_t *primes = NULL;  /* Array of primes */
static size_t prime_cnt = 0;     /* Number of primes in primes */
static bool primes_ready = false;

/* Ensure the prime table is ready.  A fixed upper bound is enough for
   n <= 10000 – the 10000th squarefree number is < 20000, so primes up
   to 200000 are more than sufficient. */
static void ensure_primes(void)
{
    if (primes_ready) return;
    /* 200000 comfortably exceeds sqrt(20000). */
    const idx_t limit = 200000ULL;
    build_primes_upto(limit, &primes, &prime_cnt);
    primes_ready = true;
}

/* Ensure the cache holds at least [0..n] entries. */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap) return;
    size_t new_cap = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);        /* allocation failure */
    /* Initialise new entries. */
    for (idx_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return true iff n is square‑free (no repeated prime factor). */
static bool is_squarefree_uint64(uint64_t n)
{
    if (n <= 1) return true;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) {
            n /= p;
            if (n % p == 0) return false; /* p^2 divides original n */
            while (n % p == 0) n /= p;    /* remove possible further powers */
        }
    }
    return true;
}

/* Return the nth term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 */

    ensure_cache(n);
    if (n <= cached_max) return cache[n];

    /* Prepare primes once. */
    ensure_primes();

    static uint64_t last_sqfree = 0;   /* last square‑free number found */

    for (idx_t i = cached_max + 1; i <= n; ++i) {
        uint64_t candidate = last_sqfree + 1;
        while (!is_squarefree_uint64(candidate))
            ++candidate;
        last_sqfree = candidate;

        seq_t prod = 1;
        uint64_t temp = candidate;
        for (size_t pi = 0;
             pi < prime_cnt && primes[pi] * primes[pi] <= temp;
             ++pi) {
            uint64_t p = primes[pi];
            if (temp % p == 0) {
                prod *= (seq_t)((p * p) - 1);
                while (temp % p == 0) temp /= p;
            }
        }
        if (temp > 1) {      /* Remaining prime factor larger than sqrt. */
            prod *= (seq_t)((temp * temp) - 1);
        }

        cache[i] = prod;
    }

    cached_max = n;
    return cache[n];
}
