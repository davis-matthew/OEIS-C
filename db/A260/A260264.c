/*
 * A260264  a(n+1) = a(n) + largest digit not in a(n), starting with a(0) = 0.
 * https://oeis.org/A260264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0; a(n+1) = a(n) + largest digit not in a(n) (digits
 *            0..9)
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   largest_missing_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/largest_missing_digit.h"

/* Cache to enable arbitrary access order and memoization. */
static seq_t *cache      = NULL;  /* array of computed terms */
static size_t cache_cap  = 0;     /* allocated capacity   */
static idx_t   last_idx   = 0;    /* highest index stored (>=0) */
static bool    constant_reached = false;
static idx_t   constant_index   = 0;
static seq_t   constant_value   = 0;

/* Ensure the cache can hold index n. */
static void ensure_capacity(idx_t n) {
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 16) new_cap = 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Main sequence accessor. */
seq_t a(idx_t n) {
    /* Initial entry for a(0) */
    if (cache == NULL) {
        ensure_capacity(0);
        cache[0] = (seq_t)0;
        last_idx = 0;
    }

    /* If we already know the value from the constant tail, just return it. */
    if (constant_reached && n >= constant_index) {
        return constant_value;
    }

    /* Grow the cache if necessary. */
    if ((size_t)n + 1 > cache_cap) {
        ensure_capacity(n);
    }

    /* Compute missing terms up to n, unless we hit the constant tail. */
    for (idx_t i = last_idx; i < n; ++i) {
        seq_t curr = cache[i];
        unsigned missing = largest_missing_digit(curr);
        seq_t next = curr + (seq_t)missing;
        cache[i + 1] = next;

        /* Check for the constant terminal condition. */
        if (!constant_reached && next == curr) {
            constant_reached = true;
            constant_index   = i + 1;
            constant_value   = next;
            last_idx         = i + 1;
            break;
        }
    }

    if (!constant_reached) {
        last_idx = n;
    }

    return cache[n];
}
