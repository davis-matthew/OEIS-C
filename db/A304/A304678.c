/*
 * A304678  Numbers with weakly increasing prime multiplicities.
 * https://oeis.org/A304678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer whose prime multiplicities
 *            (the exponents in its prime factorisation) are weakly increasing
 * Range:     u8 n <= 189, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   weakly_increasing
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/weakly_increasing.h"

/* cache of already produced terms */
static seq_t *cache   = NULL;
static idx_t  cache_len = 0;

/* next integer to be tested (starts from 2 after 1 has been cached) */
static uint64_t next_candidate = 2;

/* Grow the cache if necessary. */
static void ensure_cache(idx_t need)
{
    if (cache_len >= need)
        return;
    size_t new_cap = (cache_len == 0) ? 8 : cache_len * 2;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();            /* out of memory – terminate */
    cache = new_cache;
}

/* Add a newly found term to the cache. */
static void add_to_cache(seq_t value)
{
    ensure_cache(cache_len + 1);
    cache[cache_len++] = value;
}

/* Return the n-th term (a(1) is the first term). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;         /* undefined; not used by tests */

    /* Initialise cache with the first term 1 */
    if (cache_len == 0) {
        ensure_cache(8);
        cache[0] = (seq_t)1;
        cache_len = 1;
        next_candidate = 2;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        while (!weakly_increasing(next_candidate))
            ++next_candidate;
        add_to_cache((seq_t)next_candidate);
        ++next_candidate;
    }

    return cache[n - 1];
}
