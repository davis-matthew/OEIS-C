/*
 * A343314  a(n) is the number of nonnegative integers that can be represented
 *          in a 7-segment display by using only n segments (version A063720).
 * https://oeis.org/A343314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-2) + a(n-3) + a(n-4) + 5*a(n-5) + a(n-6) + a(n-7) for
 *            n >= 14 Initial terms:
 *            a(0)=0,a(1)=0,a(2)=1,a(3)=1,a(4)=2,a(5)=7,a(6)=5,
 *            a(7)=16,a(8)=19,a(9)=39,a(10)=77,a(11)=103,a(12)=226,a(13)=334
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init_vals[14] = {
    0, 0, 1, 1, 2, 7, 5, 16, 19, 39, 77, 103, 226, 334
};

static seq_t *cache          = NULL;   /* storage for computed values */
static size_t cache_cap      = 0;      /* allocated capacity (in elements) */
static size_t cache_len      = 0;      /* number of elements actually filled (next free index) */

/* Allocate or grow the cache to at least 'need' elements.
   Does not alter existing contents.  Returns 1 on success, 0 on failure. */
static int ensure_cache(size_t need)
{
    if (cache_cap >= need)
        return 1;                     /* already big enough */

    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap < need)
        new_cap <<= 1;                 /* double until big enough */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        return 0;                     /* allocation failed */

    cache = tmp;
    cache_cap = new_cap;
    return 1;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    /* First time: create the cache and load the 14 known values. */
    if (!cache)
    {
        if (!ensure_cache(64))
            return (seq_t)0;          /* allocation failure – unlikely in tests */
        /* copy the initial sequence into the cache */
        for (size_t i = 0; i < 14; ++i)
            cache[i] = init_vals[i];
        cache_len = 14;
    }

    /* If the requested value is already computed, return it. */
    if (idx < cache_len)
        return cache[idx];

    /* Compute all missing terms up to idx. */
    while (cache_len <= idx)
    {
        /* Recurrence requires that cache_len >= 14; the loop guard guarantees this. */
        seq_t val = cache[cache_len - 2] + cache[cache_len - 3] +
                    cache[cache_len - 4] + 5 * cache[cache_len - 5] +
                    cache[cache_len - 6] + cache[cache_len - 7];
        if (!ensure_cache(cache_len + 1))
            return (seq_t)0;          /* allocation failure – unlikely in tests */
        cache[cache_len] = val;
        ++cache_len;
    }

    return cache[idx];
}
