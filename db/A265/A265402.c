/*
 * A265402  Fixed points of A265388: numbers n for which gcd{k=1..n-1}
 *          binomial(2*n, 2*k) = n.
 * https://oeis.org/A265402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Fixed points of A265388: numbers n for which gcd{k=1..n-1}
 *            binomial(2*n, 2*k) = n. These are exactly the primes p such that
 *            2p-1 has at least two distinct prime factors
 * Range:     u8 n <= 36, u16 n <= 5680, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/count_distinct_primes.h"

/* Cache of pre‑computed terms */
static seq_t *cache = NULL;      /* array of sequence values */
static size_t cache_size = 0;    /* number of terms stored */
static size_t cache_cap   = 0;    /* allocated capacity */
static idx_t  last_candidate = 3; /* last number that was examined */

/* Helper to generate terms up to the requested index */
static void generate_terms(size_t need)
{
    if (cache_size >= need) return;

    /* initialise cache if first time */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) abort();
    }

    while (cache_size < need) {
        ++last_candidate;          /* move to next integer */
        if (!is_prime(last_candidate)) continue; /* only primes are possible */

        /* Test if 2*p-1 has at least two distinct prime factors */
        idx_t t = 2 * last_candidate - 1;
        if (count_distinct_primes(t) < 2) continue; /* does not qualify */

        /* store the term, expanding cache if needed */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (!new_cache) abort();
            cache = new_cache;
            cache_cap = new_cap;
        }
        cache[cache_size++] = (seq_t)last_candidate;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of bounds, return 0 */
    /* ensure cache has at least n terms */
    generate_terms((size_t)n);
    return cache[(size_t)n - 1];
}
