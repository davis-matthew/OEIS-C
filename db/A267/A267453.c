/*
 * A267453  Number of OFF (white) cells in the n-th iteration of the "Rule
 *          131" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A267453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3)+a(n-4)-a(n-7) for n>6; a(0)=0, a(1)=2, a(2)=3,
 *            a(3)=5, a(4)=5, a(5)=8, a(6)=7
 * Range:     u8 n <= 202, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* initial values – guaranteed to be needed for all n */
static const seq_t init_vals[7] = {
    (seq_t)0,
    (seq_t)2,
    (seq_t)3,
    (seq_t)5,
    (seq_t)5,
    (seq_t)8,
    (seq_t)7
};

/* memoisation cache */
static seq_t *cache = NULL;
static idx_t    cache_cap = 0;

/* grow the cache so that indices up to n can be stored */
static void ensure_capacity(idx_t n)
{
    if (cache_cap > n)
        return;

    idx_t new_cap = n + 1;

    seq_t *new_cache = NULL;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
#else
    new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif

    /* copy old data (if any) */
    for (idx_t i = 0; i < cache_cap; ++i)
        new_cache[i] = cache[i];

    /* initialize the first seven entries if they were not yet set */
    for (idx_t i = cache_cap; i < 7 && i <= n; ++i)
        new_cache[i] = init_vals[i];

    /* free the old block (correctly for C or C++) */
#ifdef __cplusplus
    delete [] cache;
#else
    free(cache);
#endif

    cache     = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* for the first seven terms return the hard‑coded values */
    if (n < 7)
        return init_vals[(size_t)n];

    /* remember where we stopped before resizing */
    idx_t prev_cap = cache_cap;

    /* make sure the cache can hold index n */
    ensure_capacity(n);

    /* compute new terms only if they are not already known */
    idx_t start = (prev_cap > 7) ? prev_cap : 7;
    for (idx_t i = start; i <= n; ++i)
    {
        cache[i] = cache[i - 3] + cache[i - 4] - cache[i - 7];
    }

    return cache[n];
}
