/*
 * A295075  Numbers k such that d + k/d is never prime for any divisor d of k.
 * https://oeis.org/A295075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 175, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches for the n-th integer k such that for every divisor d of k,
   the sum d + k/d is never prime.  The function builds a cache of
   found values and returns the (n-1)-indexed element.  It uses simple
   divisor enumeration; thus it is O(N sqrt N) but fast enough for
   n ≤ 10 000. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"

static idx_t *cache = NULL;          /* cache of found terms */
static idx_t cache_len = 0;          /* number of elements stored */
static idx_t cache_cap = 0;          /* allocated capacity */
static uint64_t last_checked = 0;    /* last number examined */

/* Grow the cache to at least 'needed' slots.  Safe for both C and C++ */
static void ensure_cache_size(idx_t needed)
{
    if (cache_cap >= needed) return;
    idx_t new_cap = cache_cap > 0 ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    idx_t *tmp = (idx_t *)realloc(cache, sizeof(idx_t) * new_cap);
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Return true iff all divisor sums d + k/d are composite (or 1). */
static bool satisfies(uint64_t k)
{
    for (uint64_t d = 1; d * d <= k; ++d) {
        if (k % d == 0) {
            seq_t s = (seq_t)d + (seq_t)(k / d);
            if (isPrimeGeneric(s)) return false;
        }
    }
    return true;
}

/* Public sequence function required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n=0, return 0 safely */
    /* Ensure cache holds at least n elements */
    while (cache_len < n) {
        ensure_cache_size(cache_len + 64);
        ++last_checked;
        if (satisfies(last_checked))
            cache[cache_len++] = last_checked;
    }
    return (seq_t)cache[n - 1];
}
