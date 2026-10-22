/*
 * A386358  Primes without {7, 9} as digits.
 * https://oeis.org/A386358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1724, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes without the digits 7 or 9, produced by a simple
   incremental search and cached.
   a(1) = 2, a(2) = 3, a(3) = 5, ...      */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t n); */

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;
static uint64_t next_candidate = 2;

/* Return true iff all decimal digits of val are not 7 or 9. */
static bool allowed_digits(uint64_t val)
{
    while (val > 0) {
        uint64_t d = val % 10;
        if (d == 7 || d == 9) return false;
        val /= 10;
    }
    return true;
}

/* Grow the cache to hold at least 'needed' entries.  */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();   /* out of memory */
    cache       = new_cache;
    cache_cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out-of-range guard, unused by OEIS */

    /* Build the cache until it contains at least n terms */
    while (cache_size < n) {
        uint64_t cand = next_candidate++;
        if (!allowed_digits(cand)) continue;
        if (!is_prime(cand))      continue;

        ensure_cache(cache_size + 1);
        cache[cache_size++] = (seq_t)cand;
    }
    return cache[n - 1];
}
