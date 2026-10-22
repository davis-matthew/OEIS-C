/*
 * A343760  Numbers whose digits can be the lengths of the sides of a polygon.
 * https://oeis.org/A343760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th positive integer whose decimal digits * are all
 *            non‑zero and satisfy that for each digit d, * d <
 *            sum_of_other_digits (polygon inequality)
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_polygon_number
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_polygon_number.h"

/* Cache for already found terms */
static seq_t *cache   = NULL;   /* dynamic array of terms */
static size_t cache_len = 0;    /* number of stored terms */
static size_t cache_cap = 0;    /* allocated capacity */

/* Generator state: next integer to test, and how many terms have been collected */
static seq_t next_candidate = (seq_t)111;
static idx_t terms_collected = 0;

/* Ensure that the cache has room for one more term */
static void ensure_cache_capacity(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_buf) abort();          /* fatal error on OOM */
        cache = new_buf;
        cache_cap = new_cap;
    }
}

/* Return the n-th term of the sequence (1‑based).  Undefined for n < 1. */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* safety for out‑of‑range call */

    /* Grow the cache until we have at least n terms */
    while (terms_collected < n) {
        if (is_polygon_number(next_candidate)) {
            ensure_cache_capacity();
            cache[cache_len++] = next_candidate;
            ++terms_collected;
        }
        ++next_candidate;
    }

    return cache[n - (idx_t)1];
}
