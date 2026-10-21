/*
 * A274050  Numbers that cannot be expressed as the sum of two nonzero squares
 *          and at most one power of two.
 * https://oeis.org/A274050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 9418, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A274050: numbers that cannot be expressed as the sum of
   two nonzero squares and at most one power of two.  
   We precompute all sums a² + b² (a,b>0) up to a dynamic limit and
   all powers of two ≤ that limit.  For each candidate number cur we
   test whether it is representable as such a sum or as that sum
   minus a power of two.  The first n non‑representable numbers are
   cached so that the function works for calls in any order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static tables that grow as needed */
static idx_t  limit   = 0;           /* maximum number for which tables exist          */
static uint8_t *sum_sq = NULL;       /* sum_sq[x]==1 iff x = a²+b², a,b>0                */
static idx_t  *powers = NULL;       /* array of powers of two ≤ limit                 */
static size_t  power_cnt = 0;

/* Cache of the sequence values already found */
static seq_t   *cache  = NULL;
static idx_t   cache_size = 0, cache_cap = 0;

/* Helper to enlarge the cache when needed. */
static void add_to_cache(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 256;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* Rebuild the sum_sq bitset and powers array up to new_limit. */
static void extend_limit(idx_t new_limit)
{
    if (new_limit <= limit) return;

    /* Allocate new, zeroed bitset. */
    uint8_t *new_sq = (uint8_t *)calloc((size_t)(new_limit + 1), sizeof(uint8_t));
    if (!new_sq) exit(EXIT_FAILURE);

    /* Compute all a² + b² with a,b>0 and a² + b² ≤ new_limit. */
    for (idx_t a = 1; a * a <= new_limit; ++a) {
        idx_t aa = a * a;
        for (idx_t b = a; aa + b * b <= new_limit; ++b) {
            new_sq[aa + b * b] = 1;
        }
    }

    /* Recompute powers of two. */
    free(powers);
    powers   = NULL;
    power_cnt = 0;
    for (idx_t p = 1; p <= new_limit; p <<= 1) {
        idx_t *tmp = (idx_t *)realloc(powers, (power_cnt + 1) * sizeof(idx_t));
        if (!tmp) exit(EXIT_FAILURE);
        powers = tmp;
        powers[power_cnt++] = p;
    }

    /* Replace old table. */
    free(sum_sq);
    sum_sq = new_sq;
    limit  = new_limit;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Ensure we have at least n terms cached. */
    static idx_t cur = 1;               /* next candidate number to examine */
    while (cache_size < n) {
        /* Grow tables if necessary. */
        if (cur > limit)
            extend_limit((cur > limit) ? cur * 2 : 4);

        /* Check if cur is representable. */
        bool rep = sum_sq[cur];
        if (!rep) {
            for (size_t i = 0; i < power_cnt; ++i) {
                idx_t p = powers[i];
                if (cur > p && sum_sq[cur - p]) {
                    rep = true;
                    break;
                }
            }
        }

        /* If not representable, store it. */
        if (!rep)
            add_to_cache((seq_t)cur);

        ++cur;
    }

    /* Return the requested term (1‑based indexing). */
    return cache[n - 1];
}
