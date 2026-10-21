/*
 * A241145  Numbers n such that n and 9n share at least one digit.
 * https://oeis.org/A241145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   share_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A241145: numbers n such that n and 9n share at least one digit. */
/* This implementation performs a linear, incremental search and caches the terms. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/share_digits.h"

/* Static cache of computed terms. */
static seq_t *cached_terms = NULL;
static size_t cached_size = 0;
static size_t cached_cap  = 0;
static const size_t INITIAL_CAP = 256;
static seq_t next_candidate = 1;

/* Ensure the cache can hold at least 'need' terms. */
static void ensure_cache_cap(size_t need)
{
    if (cached_cap >= need)
        return;
    size_t new_cap = cached_cap ? cached_cap * 2 : INITIAL_CAP;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_arr = (seq_t *)realloc(cached_terms, new_cap * sizeof(*cached_terms));
    if (!new_arr) {
        /* Allocation failed – halt with an abort. */
        abort();
    }
    cached_terms = new_arr;
    cached_cap  = new_cap;
}

/* The required function. */
seq_t a(idx_t n)
{
    /* Offset is 1.  For n == 0 simply return 0 (undefined). */
    if (n == 0)
        return (seq_t)0;

    /* Make sure the cache is large enough to hold the nth term. */
    ensure_cache_cap((size_t)n);

    /* Generate terms until we have at least n terms. */
    while (cached_size < (size_t)n) {
        seq_t val = next_candidate;
        seq_t nine_val = val * 9;
        if (share_digits(val, nine_val))
            cached_terms[cached_size++] = val;
        next_candidate++;
    }

    return cached_terms[(size_t)(n - 1)];
}
