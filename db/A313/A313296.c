/*
 * A313296  Coordination sequence Gal.6.209.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12 Initial values (0 ≤
 *            n ≤ 12): a(0)=1, a(1)=4, a(2)=9, a(3)=15, a(4)=21, a(5)=27,
 *            a(6)=33, a(7)=39, a(8)=45, a(9)=51, a(10)=56, a(11)=60, a(12)=64
 *            (Implementation uses a memoised linear recurrence.)
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, NULL */
#include <stddef.h>   /* for size_t */

static seq_t *cache        = NULL;   /* dynamically growing array of seq_t values */
static size_t cache_used   = 0;      /* number of values stored (next free index ) */
static size_t cache_cap    = 0;      /* current allocation size */

/* Internal helper: ensure the cache holds values up to index n. */
static void
ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_used)
        return;                            /* already computed */

    /* Expand cache if necessary */
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        if (new_cap < needed)
            new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                  /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Fill initial values (indices 0..12) if not already present */
    if (cache_used == 0) {
        static const seq_t init[13] = {
            (seq_t)1,  (seq_t)4,  (seq_t)9,  (seq_t)15,
            (seq_t)21, (seq_t)27, (seq_t)33, (seq_t)39,
            (seq_t)45, (seq_t)51, (seq_t)56, (seq_t)60,
            (seq_t)64
        };
        for (size_t i = 0; i < 13; ++i)
            cache[i] = init[i];
        cache_used = 13;
    }

    /* Compute missing values using the recurrence */
    for (size_t i = cache_used; i < needed; ++i) {
        if (i <= 12) continue;                /* already set above */
        /* a(i) = a(i-1) + a(i-11) - a(i-12) */
        cache[i] = cache[i - 1] + cache[i - 11] - cache[i - 12];
    }
    cache_used = needed;
}

/* Public API */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
