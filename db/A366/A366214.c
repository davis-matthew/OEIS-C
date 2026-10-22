/*
 * A366214  Numbers whose median of the digits is equal to 8.
 * https://oeis.org/A366214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   median_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of OEIS A366214: numbers whose median of the digits is 8.
   The function uses a simple enumeration of increasing integers and stores
   all qualifying numbers in a dynamically resized array, so subsequent
   calls for any n in any order are answered in O(n). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/median_ok.h"

/* Static cache of generated terms */
static seq_t *cache = NULL;          /* array of terms */
static size_t cache_len = 0;         /* number of stored terms */
static size_t cache_cap = 0;         /* allocated capacity */

/* Last integer that has been checked (in seq_t type) */
static seq_t last_checked = (seq_t)7;

/* Extend the cache until it contains at least target terms */
static void ensure_cache(idx_t target)
{
    if (cache_len >= (size_t)target)
        return;

    /* Extend the internal array as needed */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    for (;;) {
        /* Enumerate the next integer */
        last_checked = last_checked + 1;

        if (median_ok(last_checked)) {
            if (cache_len >= cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = last_checked;
        }

        if (cache_len >= (size_t)target)
            break;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined, but safe */
        return 0;

    ensure_cache(n);
    return cache[n - 1];
}
