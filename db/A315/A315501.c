/*
 * A315501  Coordination sequence Gal.4.149.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-5) - a(n-10) for n > 13 Initial values (a(0) …
 *            a(13)) are taken from the OEIS entry
 * Range:     u8 n <= 43, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache of computed values */
static seq_t *cache = NULL;          /* array of computed terms */
static idx_t  cache_len  = 0;        /* number of values stored (0..cache_len-1) */
static idx_t  cache_cap  = 0;        /* allocated capacity */

/* first 14 terms, indexed 0 … 13 */
static const seq_t BASE[14] = {
    1, 6, 11, 16, 23, 28, 36, 40, 45, 53,
    56, 66, 68, 73
};

/* initialise the cache to hold at least the first 14 values */
static void init_cache(void)
{
    if (cache != NULL)
        return;                      /* already initialised */

    /* allocate a small initial capacity */
    cache_cap = 16;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) { /* allocation failure – abort program */
        abort();
    }

    /* copy the known initial values */
    for (idx_t i = 0; i < 14; ++i)
        cache[i] = BASE[i];

    cache_len = 14;                  /* we now know indices 0 … 13 */
}

/* grow the cache so that indices up to n are available and
   compute any new values using the recurrence. */
static void ensure_cache_size(idx_t n)
{
    init_cache();

    if (n + 1 <= cache_len)          /* already have what we need */
        return;

    /* increase capacity if necessary */
    while (cache_cap <= n) {
        cache_cap <<= 1;             /* double */
    }
    seq_t *new_cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!new_cache) { /* allocation failure – abort */
        abort();
    }
    cache = new_cache;

    /* compute missing values in order */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence holds for i >= 14 (since 0‑based index 14 == a(14)) */
        cache[i] = (seq_t)(2 * cache[i - 5] - cache[i - 10]);
    }

    cache_len = n + 1;               /* now indices 0 … n are valid */
}

/* the requested sequence function (external linkage) */
seq_t a(idx_t n)
{
    ensure_cache_size(n);
    return cache[n];
}
