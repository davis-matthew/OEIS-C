/*
 * A282913  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 526", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A282913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 16*a(n-4) - 16*a(n-5) for n > 5 with initial
 *            values a(0)=1, a(1)=3, a(2)=7, a(3)=13, a(4)=25, a(5)=59
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   alloc_seq_array, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t, NULL */
#include <stdint.h>   /* uint64_t */

/* The utilities allow allocation for arbitrary-precision seq_t in both
   C and C++.  For C++ they use new; for C they use malloc. */
#include "../utils/alloc_seq_array.h"    /* seq_t *alloc_seq_array(size_t) */
#include "../utils/free_cache.h"          /* void free_cache(seq_t *) */

static seq_t *cache      = NULL;   /* dynamically grown array of terms */
static idx_t  cache_size = 0;      /* number of terms currently stored */
static idx_t  cache_cap  = 0;      /* allocated capacity */

/* first six terms of the sequence */
static const seq_t init_vals[6] = {
    (seq_t)1,   /* a(0) */
    (seq_t)3,   /* a(1) */
    (seq_t)7,   /* a(2) */
    (seq_t)13,  /* a(3) */
    (seq_t)25,  /* a(4) */
    (seq_t)59   /* a(5) */
};

/* grow the cache to hold at least n+1 terms, if necessary */
static void
ensure_cache(idx_t n)
{
    /* sufficient capacity? */
    if (cache_cap <= (size_t)n)
    {
        /* decide new capacity : double or at least n+1 */
        idx_t new_cap = cache_cap > 0 ? cache_cap * 2 : 16;
        if (new_cap <= (size_t)n + 1)
            new_cap = (size_t)n + 1;

        /* allocate new array */
        seq_t *new_cache = alloc_seq_array((size_t)new_cap);
        if (!new_cache)
        {
            /* allocation failed – abort (caller cannot recover) */
            abort();
        }

        /* copy existing elements if any */
        for (idx_t i = 0; i < cache_size; ++i)
        {
            new_cache[i] = cache[i];
        }

        /* free old array if it existed */
        if (cache)
        {
            free_cache(cache);
        }

        cache      = new_cache;
        cache_cap  = new_cap;
    }

    /* initialise the first six entries if not already present */
    if (cache_size < 6)
    {
        for (idx_t i = cache_size; i < 6; ++i)
        {
            cache[i] = init_vals[i];
        }
        cache_size = 6;
    }

    /* compute terms from cache_size up to n */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        if (i < 6)
        {
            /* already initialised */
            continue;
        }
        seq_t t1 = cache[i-1];
        seq_t t2 = cache[i-4] * (seq_t)16;
        seq_t t3 = cache[i-5] * (seq_t)16;
        cache[i] = t1 + t2 - t3;
    }

    /* update number of stored terms */
    if (cache_size <= n)
        cache_size = n + 1;
}

seq_t a(idx_t n)
{
    /* For n < 6 we can return immediately */
    if (n < 6)
        return init_vals[n];

    /* Ensure we have computed up to n */
    ensure_cache(n);

    return cache[n];
}
