/*
 * A297136  Numbers whose base-6 digits d(m), d(m-1), ..., d(0) have m=0 or
 *          else d(i) = d(i+1) for some i in {0,1,...,m-1}.
 * https://oeis.org/A297136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the nth positive integer whose base‑6 representation *
 *            either has a single digit (m=0) or contains at least one pair of
 *            * consecutive equal digits
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_consecutive_equal_digits_base6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_consecutive_equal_digits_base6.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;
static seq_t last_checked = 0;

/* Ensure the cache contains at least n elements.  This appends new
 * terms incrementally from the current last_checked value.  The cache
 * is stored in ascending order. */
static void ensure_cache(idx_t n)
{
    if (cache_size >= (size_t)n)
        return;

    /* Grow cache.  Use a factor of 2 to keep amortised linear time. */
    size_t new_size = cache_size == 0 ? 1 : cache_size << 1;
    if (new_size < (size_t)n)
        new_size = (size_t)n;

    seq_t *p = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!p)
        abort();               /* out of memory – terminate */
    cache = p;

    seq_t cand = last_checked + (seq_t)1;
    for (size_t i = cache_size; i < new_size; ++i)
    {
        while (!has_consecutive_equal_digits_base6(cand))
            cand += (seq_t)1;
        cache[i] = cand;
        cand += (seq_t)1;
    }
    last_checked = cache[new_size - 1];
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0)
        abort();               /* sequence is 1-indexed */
    ensure_cache(n);
    return cache[n - 1];
}
