/*
 * A293880  Numbers having '20' as substring of their digits.
 * https://oeis.org/A293880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A293880: numbers having '20' as a decimal substring. */
/* a(n) = the nth positive integer whose decimal representation contains "20" */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* helper: test whether x contains the substring "20" */
static bool contains20(seq_t x)
{
    if (x < 20) return false;
    seq_t last = x % 10;
    while (x >= 10) {
        x /= 10;
        seq_t next = x % 10;
        if (next == 2 && last == 0) return true;
        last = next;
    }
    return false;
}

/* static cache for memoization */
static seq_t *cache = NULL;          /* stored elements */
static size_t cache_len = 0;         /* number of elements stored */
static size_t cache_cap = 0;         /* allocated capacity */

/* ensure cache has at least 'needed' elements of space */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 32;
    while (needed > new_cap) new_cap <<= 1;
    cache = mem_resize(seq_t, cache, new_cap);
    cache_cap = new_cap;
}

/* public entry point */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* offset starts at 1 */
    if (cache_len >= (size_t)n)
        return cache[(size_t)n - 1];

    /* find next values until we have at least n terms */
    seq_t current = cache_len ? cache[cache_len - 1] : 0;
    while (cache_len < (size_t)n) {
        current++;
        if (contains20(current)) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = current;
        }
    }
    return cache[(size_t)n - 1];
}
