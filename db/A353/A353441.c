/*
 * A353441  Integers m such that the decimal expansion of 1/m contains the
 *          digit 5.
 * https://oeis.org/A353441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A353441 – the nth integer m such that 1/m has a decimal digit 5
 *            in its expansion (offset = 1). This code incrementally searches
 *            m, caching results in a static table so that repeated calls are
 *            fast. It uses an idx_t array for the cache, which works for all
 *            builds (including the arbitrary‑precision...
 * Range:     u8 n <= 194, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_digit_5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_digit_5.h"

/* Static cache of found terms, stored as idx_t to avoid constructor
   issues in the arbitrary‑precision build. */
static idx_t *cache = NULL;
static idx_t  cache_len = 0;          /* number of cached terms */
static idx_t  cache_cap = 0;          /* allocated capacity */
static idx_t  last_checked = 1;       /* next candidate m to test */

/* Grow the cache to at least new_cap elements.  Uses malloc/realloc
   which work for idx_t (i.e., uint64_t).  On the first allocation
   we use a small default size. */
static void ensure_cache_cap(idx_t new_cap)
{
    if (cache_cap >= new_cap)
        return;
    idx_t cap = new_cap > cache_cap * 2 ? new_cap : cache_cap * 2;
    if (cap == 0)
        cap = 8;
    idx_t *tmp = (idx_t *)realloc(cache, cap * sizeof(idx_t));
    if (!tmp)
        return;   /* allocation failed – leave cache unchanged */
    cache = tmp;
    cache_cap = cap;
}

/* Append a new term to the cache. */
static void cache_append(idx_t val)
{
    ensure_cache_cap(cache_len + 1);
    cache[cache_len++] = val;
}

/* Main sequence function.  The function is safe for any order of
   calls.  For n ≤ 0 it returns 0 (outside the defined offset). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset is 1 */

    /* If already cached, return the stored value. */
    if (n <= cache_len)
        return (seq_t)cache[n - 1];

    /* Otherwise extend the cache until it contains at least n terms. */
    while (cache_len < n)
    {
        ++last_checked;
        if (has_digit_5((seq_t)last_checked))
            cache_append(last_checked);
    }
    return (seq_t)cache[n - 1];
}
