/*
 * A373046  Number of binary strings of length n that contain the substring
 *          1000.
 * https://oeis.org/A373046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-3) + a(n-1) + a(n-2) + a(n-3) - 1 for n ≥ 7 Initial
 *            values: a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=4, a(6)=12
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 941 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>               /* abort() */
#include <stdbool.h>
#include "../utils/pow2.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Static cache (array of seq_t)  */
static seq_t *cache      = NULL;   /* array of computed values                */
static idx_t  cache_len  = 0;      /* current allocated size (elements)       */
static idx_t  max_computed = 6;    /* highest index that has been computed    */
static int    init_done  = 0;      /* flag to initialise first 7 values once */

/* Ensure that the cache has room for indices up to n (inclusive). */
static void
ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;                            /* already large enough */

    /* Determine a new size, at least 8 and a power‑of‑two > n. */
    idx_t new_len = (cache_len == 0) ? 8 : cache_len;
    while (new_len <= n)
        new_len <<= 1;

    /* Allocate or grow the array.  Use helpers that are safe for
       arbitrary‑precision seq_t types. */
    seq_t *new_cache = NULL;
    if (cache == NULL) {
        new_cache = seq_alloc(new_len);
    } else {
        new_cache = seq_realloc(cache, cache_len, new_len);
    }
    if (!new_cache)
        abort();                          /* out of memory; very unlikely */

    cache      = new_cache;
    cache_len  = new_len;

    /* Initialise the first seven values only once. */
    if (!init_done) {
        static const seq_t initvalues[7] = {0, 0, 0, 0, 1, 4, 12};
        for (idx_t i = 0; i < 7; ++i)
            cache[i] = initvalues[i];
        init_done = 1;
        /* max_computed already 6, consistent with the init table. */
    }
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    /* Make sure the cache can store value at index n. */
    ensure_cache(n);

    /* If the value has already been computed, just return it. */
    if (n <= max_computed)
        return cache[n];

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        /* a(k) = 2^(k-3) + a(k-1) + a(k-2) + a(k-3) - 1 */
        seq_t term = pow2(k - 3);
        term += cache[k - 1];
        term += cache[k - 2];
        term += cache[k - 3];
        term -= (seq_t)1;
        cache[k] = term;
    }
    max_computed = n;

    return cache[n];
}
