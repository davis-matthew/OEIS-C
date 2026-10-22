/*
 * A327906  Numbers with only one factorization into factors > 1 with integer
 *          mean (namely, as a singleton).
 * https://oeis.org/A327906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_integer_mean_factorization
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of integers n > 1 such that all factorizations into factors > 1
   have non‑integer mean except the singleton factorization.  The function
   returns the n‑th term of that sequence. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_integer_mean_factorization.h"

/* Utility: test whether a uint64_t is prime (trial division) */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
    {
        if (n % d == 0) return false;
    }
    return true;
}

/* Determine whether n has only the singleton factorization with integer mean. */
static bool satisfies(uint64_t n)
{
    if (n <= 1) return false;          /* 1 is excluded by the OEIS offset */

    /* Primes automatically qualify (no non‑trivial factorization). */
    if (is_prime_u64(n))
        return true;

    /* Check for any other factorization with integer mean. */
    return !has_integer_mean_factorization(n, 0, 0, 2);
}

/* Cached sequence storage */
static seq_t *cache      = NULL;   /* array of generated terms */
static size_t cache_len  = 0;      /* number of stored terms */
static size_t cache_cap  = 0;      /* allocated capacity of cache */
static uint64_t last_candidate = 1;/* integer value of the last generated term */

/* Grow the cache array to accommodate at least `needed` elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    while (new_cap < needed) new_cap *= 2;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (new_cache == NULL) exit(EXIT_FAILURE); /* out of memory */
    cache     = new_cache;
    cache_cap = new_cap;
}

/* Main sequence function called by the harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined offset, sentinel */
    if ((size_t)n <= cache_len)
        return cache[n - 1];           /* cached value */

    /* Generate terms until we reach the required index. */
    while (cache_len < (size_t)n)
    {
        ++last_candidate;
        if (satisfies(last_candidate))
        {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len] = (seq_t)last_candidate;
            ++cache_len;
        }
    }

    return cache[n - 1];
}
