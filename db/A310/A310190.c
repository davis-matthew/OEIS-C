/*
 * A310190  Coordination sequence Gal.5.19.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A310190: Coordination sequence Gal.5.19.2
   Recurrence: a(n) = a(n-1) - a(n-4) + a(n-5) + a(n-8) - a(n-9) + a(n-12) - a(n-13)
   for n > 13, with initial values a(0)=1, a(1)=3, a(2)=7, a(3)=10, a(4)=14,
   a(5)=20, a(6)=21, a(7)=27, a(8)=28, a(9)=31, a(10)=37, a(11)=38,
   a(12)=44, a(13)=50.
   The implementation uses memoization to allow arbitrary order of calls.
*/

#include <stdlib.h>

/* --------------------------------------------------------------------- */
/* seq_t and idx_t are defined externally in the build environment.      */
/* --------------------------------------------------------------------- */

static seq_t *cache = NULL;       /* memoized values */
static size_t cache_size = 0;     /* number of entries currently stored */
static size_t cache_cap = 0;      /* allocated capacity of cache */

/* Initial values for n = 0 .. 13 */
static const seq_t init_vals[14] = {
    (seq_t)1,  /* a(0) */
    (seq_t)3,  /* a(1) */
    (seq_t)7,  /* a(2) */
    (seq_t)10, /* a(3) */
    (seq_t)14, /* a(4) */
    (seq_t)20, /* a(5) */
    (seq_t)21, /* a(6) */
    (seq_t)27, /* a(7) */
    (seq_t)28, /* a(8) */
    (seq_t)31, /* a(9) */
    (seq_t)37, /* a(10)*/
    (seq_t)38, /* a(11)*/
    (seq_t)44, /* a(12)*/
    (seq_t)50  /* a(13)*/
};

/* Ensure cache is large enough to store values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* In production code we would check for NULL; omitted for brevity */
    cache = new_cache;
    cache_cap = new_cap;

    /* Initialise new slots with 0; not strictly necessary but safe */
    for (size_t i = cache_size; i < needed; ++i)
        cache[i] = 0;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Handle known initial values directly */
    if ((idx_t)0 <= n && n < 14u)
        return init_vals[(size_t)n];

    /* Initialise cache lazily */
    if (!cache)
    {
        /* Allocate space for at least 14 terms */
        size_t cap = 16;
        if (cap < 14)
            cap = 14;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        /* Again, NULL check omitted for brevity */
        for (size_t i = 0; i < 14; ++i)
            cache[i] = init_vals[i];
        cache_size = 14;
        cache_cap = cap;
    }

    /* Expand cache if necessary */
    ensure_cache(n);
    if ((size_t)n >= cache_size)
    {
        /* Compute terms up to n */
        for (size_t k = cache_size; k <= (size_t)n; ++k)
        {
            /* Compute a(k) using the recurrence, avoiding unsigned underflow
               by adding all positive terms first, then subtracting the sum of
               negative terms.  For all k > 13 the sum of positive terms is
               guaranteed to be ≥ the sum of negative terms. */
            seq_t pos = cache[k - 1];
            pos += cache[k - 5];
            pos += cache[k - 8];
            pos += cache[k - 12];

            seq_t neg = cache[k - 4];
            neg += cache[k - 9];
            neg += cache[k - 13];

            cache[k] = pos - neg;  /* pos >= neg, no wraparound */
        }
        cache_size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
