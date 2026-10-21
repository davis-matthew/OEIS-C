/*
 * A255276  A255275(2^n-1).
 * https://oeis.org/A255276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A255276
   Recurrence (from the given G.f.):
     a(n) = 8 a(n-1) - 16 a(n-2) - 24 a(n-3) + 145 a(n-4)
            - 236 a(n-5) + 164 a(n-6) - 24 a(n-7) - 32 a(n-8)
   with a(0)=1, a(1)=8, a(2)=28, a(3)=128, a(4)=480,
   a(5)=2008, a(6)=7776, a(7)=31672. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* Compute a(n) for any n ≥ 0, memoising intermediate values. */
seq_t a(idx_t n)
{
    /* Initial 8 terms (hard‑coded because they are needed for all n). */
    static const seq_t init_vals[8] = {
        (seq_t)1ULL,
        (seq_t)8ULL,
        (seq_t)28ULL,
        (seq_t)128ULL,
        (seq_t)480ULL,
        (seq_t)2008ULL,
        (seq_t)7776ULL,
        (seq_t)31672ULL
    };

    /* Static cache for previously computed values. */
    static seq_t *cache = NULL;          /* array of seq_t values */
    static size_t cache_cap = 0;         /* allocated capacity */
    static idx_t  cache_size = 0;        /* number of terms stored (next free index) */

    if (n < 8)                /* base case */
        return init_vals[n];

    /* Allocate or reallocate the cache so that it can hold indices up to n. */
    if (cache == NULL) {                  /* first time we need more than 8 terms */
        cache_cap = n + 1;
        if (cache_cap < 8)
            cache_cap = 8;
        cache = seq_alloc_new_array(cache_cap);
        /* initialise the first eight terms */
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = init_vals[i];
        cache_size = 8;
    } else if (cache_cap < (size_t)(n + 1)) { /* grow the cache */
        seq_t *tmp = seq_alloc_new_array((size_t)(n + 1));
        for (idx_t i = 0; i < cache_size; ++i)
            tmp[i] = cache[i];
        seq_free_array(cache);
        cache = tmp;
        cache_cap = (size_t)(n + 1);
    }

    /* Compute all missing terms up to n using the recurrence. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t term = (seq_t)8 * cache[i-1];
        term -= (seq_t)16 * cache[i-2];
        term -= (seq_t)24 * cache[i-3];
        term += (seq_t)145 * cache[i-4];
        term -= (seq_t)236 * cache[i-5];
        term += (seq_t)164 * cache[i-6];
        term -= (seq_t)24 * cache[i-7];
        term -= (seq_t)32 * cache[i-8];
        cache[i] = term;
    }
    cache_size = n + 1;                 /* all terms up to n are now stored */

    return cache[n];
}
