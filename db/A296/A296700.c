/*
 * A296700  Numbers whose base-6 digits d(m), d(m-1), ... d(0) have #(rises) =
 *          #(falls); see Comments.
 * https://oeis.org/A296700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 8186 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: first n terms of sequence A296700 (numbers whose base‑6 digits
   have equal numbers of rises and falls). */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Test whether a candidate has equal rises and falls in base‑6. */
static bool has_equal_rises_falls(idx_t x)
{
    uint8_t digits[32];   /* enough for all candidates in the tests */
    size_t len = 0;

    if (x == 0)          /* zero is not part of the sequence */
        return false;

    /* Extract digits least significant first. */
    while (x > 0) {
        digits[len++] = (uint8_t)(x % 6);
        x /= 6;
    }

    int rises = 0, falls = 0;
    for (size_t i = len; i-- > 1;) {
        uint8_t high = digits[i];
        uint8_t low  = digits[i - 1];
        if (high < low)
            ++rises;
        else if (high > low)
            ++falls;
    }
    return rises == falls;
}

/* Cache to store found terms.  The array is grown as needed. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_sz  = 0;
static idx_t next_candidate = 1;

/* Return the n‑th element (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for n = 0 */

    /* Generate terms until we have at least n of them. */
    while (cache_sz < n) {
        /* Find next candidate that satisfies the property. */
        while (!has_equal_rises_falls(next_candidate))
            ++next_candidate;

        /* Allocate or grow the cache. */
        if (cache_sz >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp =
                mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();      /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the candidate and advance. */
        cache[cache_sz++] = (seq_t)next_candidate;
        ++next_candidate;
    }

    return cache[n - 1];
}
