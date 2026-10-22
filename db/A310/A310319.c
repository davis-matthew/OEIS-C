/*
 * A310319  Coordination sequence Gal.3.4.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) + a(n-7) - a(n-8) + a(n-9) -
 *            a(n-10) with a(0)=1, a(1)=3, a(2)=9, a(3)=12, a(4)=12, a(5)=21,
 *            a(6)=27, a(7)=24, a(8)=27, a(9)=39, a(10)=42 (OEIS A310319)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[11] = {
    1,  /* a(0) */
    3,  /* a(1) */
    9,  /* a(2) */
    12, /* a(3) */
    12, /* a(4) */
    21, /* a(5) */
    27, /* a(6) */
    24, /* a(7) */
    27, /* a(8) */
    39, /* a(9) */
    42  /* a(10) */
};

static seq_t *cache      = NULL;   /* cache of computed terms */
static size_t cap       = 0;       /* number of elements currently allocated */
static idx_t  last      = 0;       /* largest index that has a stored value */

/* Ensure that the cache can hold indices up to 'needed' */
static void ensure_capacity(idx_t needed)
{
    size_t required = (size_t)needed + 1;
    if (required <= cap) return;
    size_t new_cap = cap ? cap << 1 : required;
    if (new_cap < required) new_cap = required;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();                 /* allocation failed */
    cache = new_cache;
    cap   = new_cap;
}

/* Return a(n) for any n >= 0, with memoisation */
seq_t a(idx_t n)
{
    /* Small indices are direct look‑up */
    if (n <= 10) return init_vals[n];

    /* If already computed, return the cached value */
    if (n <= last) return cache[(size_t)n];

    /* Expand the cache to the required size */
    ensure_capacity(n);

    /* Compute all missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        if (i <= 10)                      /* initialise base values */
        {
            cache[(size_t)i] = init_vals[i];
        }
        else                              /* recurrence for i > 10  */
        {
            seq_t val = cache[(size_t)(i-1)]
                      + cache[(size_t)(i-3)]
                      + cache[(size_t)(i-7)]
                      + cache[(size_t)(i-9)];
            val -= cache[(size_t)(i-2)];
            val -= cache[(size_t)(i-8)];
            val -= cache[(size_t)(i-10)];
            cache[(size_t)i] = val;
        }
    }
    last = n;
    return cache[(size_t)n];
}
