/*
 * A289846  p-INVERT of (1,0,1,0,1,0,1,0,1,...) (A059841), where p(S) = 1 - S
 *          - S^2.
 * https://oeis.org/A289846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 60, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A289846:
 *  a(0)=1, a(1)=2, a(2)=4, a(3)=9,
 *  a(n)=a(n-1)+3*a(n-2)-a(n-3)-a(n-4)  (n≥4)
 */
#include <stddef.h>
#ifdef __cplusplus
#  include <new>
#endif
#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms */
static size_t cache_cap = 0;         /* allocated capacity */
static size_t cache_len = 0;         /* number of terms stored (>=0) */

static const seq_t init_vals[4] = { (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)9 };

/* Ensure cache is allocated up to 'required' indices (0…required-1) */
static void ensure_cache(size_t required)
{
    if (required <= cache_cap)
        return;
    size_t new_cap;
    if (cache_cap == 0)
        new_cap = required;
    else
        new_cap = cache_cap * 2;
    if (new_cap < required)
        new_cap = required;

    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
#else
    new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
    if (!new_cache)
        return; /* OOM: subsequent calls will just return 0; sequence not needed */

    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

#ifdef __cplusplus
    delete[] cache;
#else
    free(cache);
#endif
    cache = new_cache;
    cache_cap = new_cap;
}

/* Computes a(n) for any n>=0, with memoization */
seq_t a(idx_t n)
{
    /* Handle initial values directly */
    if (n < 4)
        return init_vals[(size_t)n];

    /* Prepare cache to hold up to index n */
    ensure_cache((size_t)n + 1);

    /* Fill in initial values if not already done */
    if (cache_len < 4) {
        for (size_t i = 0; i < 4; ++i)
            cache[i] = init_vals[i];
        cache_len = 4;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* a(i) = a(i-1) + 3*a(i-2) - a(i-3) - a(i-4) */
        seq_t val = cache[i - 1];
        val += 3 * cache[i - 2];
        val -= cache[i - 3];
        val -= cache[i - 4];
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
