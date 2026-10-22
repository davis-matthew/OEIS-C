/*
 * A309780  Even numbers m having the property that for every odd prime
 *          divisor p of m there exists a positive integer k < p-1, such that
 *          p-k|m-k.
 * https://oeis.org/A309780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth even number m such that for every odd prime
 *            divisor p of m, m = 2*r*p with r ≥ 2 and p >
 *            smallest_prime_factor(2*r-1)
 * Range:     u8 n <= 35, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/_smallest_prime_factor.h"

/* Cache for computed terms */
static seq_t *cache      = NULL;   /* dynamic array of cached terms */
static size_t cache_len  = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* allocated capacity */

/* Next even candidate to test; starts at 20, the first term. */
static uint64_t next_candidate = 20;

/* Ensure the cache can hold at least `needed` terms. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        cache = NULL;
        cache_cap = 0;
        return; /* allocation failure – cache will rebuild on next call */
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Test whether an even integer m satisfies the required prime conditions. */
static bool is_valid_term(uint64_t m)
{
    if ((m & 1ULL) != 0) return false;          /* must be even */

    /* Extract the odd part of m. */
    uint64_t odd = m;
    while ((odd & 1ULL) == 0) odd >>= 1;
    if (odd == 1) return false;                /* no odd prime divisor */

    uint64_t temp = odd;
    while (temp > 1) {
        uint64_t p = _smallest_prime_factor(temp);
        if (p == 0) p = temp;                  /* treat remaining factor as prime */

        /* Compute r = m / (2*p); must be integer ≥ 2. */
        uint64_t r = m / (2ULL * p);
        if (r < 2ULL) return false;

        uint64_t two_r_minus1 = 2ULL * r - 1ULL;
        uint64_t q = _smallest_prime_factor(two_r_minus1);
        if (q == 0) q = two_r_minus1;          /* if odd number is prime */

        if (p <= q) return false;              /* p must exceed smallest prime of 2r-1 */

        /* Remove all copies of p from temp. */
        while (temp % p == 0) temp /= p;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined but safe guard */

    /* Generate terms up to index n. */
    while (cache_len < (size_t)n) {
        /* Advance to the next valid even candidate. */
        while (!is_valid_term(next_candidate)) next_candidate += 2ULL;

        /* Store the term. */
        ensure_cache_capacity(cache_len + 1);
        cache[cache_len++] = (seq_t)next_candidate;

        /* Prepare for the next candidate (next even after current). */
        next_candidate += 2ULL;
    }

    return cache[n - 1];
}
