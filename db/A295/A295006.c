/*
 * A295006  Numbers n such that the largest digit of n^2 is 6.
 * https://oeis.org/A295006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th integer whose square has largest decimal digit equal
 *            to 6
 * Range:     u8 n <= 3, u16 n <= 50, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   largest_digit_is_6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function scans integers in increasing order, keeps those that satisfy
   the digit condition in a cache, and returns the n‑th value. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/largest_digit_is_6.h"

/* Cache for sequence values */
static seq_t *cache = NULL;      /* array of cached terms */
static size_t cache_cap = 0;     /* current capacity of cache */
static size_t cache_size = 0;    /* number of terms currently stored */
static idx_t last_checked = 0;   /* largest integer that has been squa­red */

/* Return the n‑th term of A295006 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; value for n < 1 is irrelevant */

    /* Return from cache if we already have that many terms. */
    if (cache_size >= (size_t)n)
        return cache[n - 1];

    /* Ensure the cache has capacity for new terms. */
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Scan successive integers until we have collected n terms. */
    while (cache_size < (size_t)n) {
        ++last_checked;
        seq_t sq = (seq_t)last_checked * (seq_t)last_checked;
        if (largest_digit_is_6(sq))
            cache[cache_size++] = last_checked;
    }

    return cache[n - 1];
}
