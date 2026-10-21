/*
 * A281774  Number of distinct topologies on an n-set with exactly 6 open
 *          sets.
 * https://oeis.org/A281774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 15*a(n-1) - 85*a(n-2) + 225*a(n-3) - 274*a(n-4) + 120*a(n-5);
   Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=6, a(4)=72, a(5)=630
   Computes terms for any n ≥ 0 in O(n) time, memoising results. */
#include <stdlib.h>

static seq_t *cache = NULL;      /* memoisation array */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t  next_idx = 0;      /* index of next value to be computed */

/* Ensure cache has room for indices 0..n inclusive.  Changes only size,
   never shrinking. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;                      /* double until big enough */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                     /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;

    /* clear the new part of the array (may not be needed for unsigned types) */
    for (idx_t i = (idx_t)cache_cap; i < new_cap; ++i)
        cache[i] = 0;
}

/* Compute values up to n, storing them in cache. */
static void compute_up_to(idx_t n)
{
    if (next_idx > n)
        return;                            /* already computed */

    ensure_cache(n);

    for (idx_t i = next_idx; i <= n; ++i) {
        if (i == 0u)            cache[i] = (seq_t)0;
        else if (i == 1u)       cache[i] = (seq_t)0;
        else if (i == 2u)       cache[i] = (seq_t)0;
        else if (i == 3u)       cache[i] = (seq_t)6;
        else if (i == 4u)       cache[i] = (seq_t)72;
        else if (i == 5u)       cache[i] = (seq_t)630;
        else {
            seq_t v = (seq_t)15 * cache[i-1]
                    + (seq_t)225 * cache[i-3]
                    + (seq_t)120 * cache[i-5]
                    - (seq_t)85  * cache[i-2]
                    - (seq_t)274 * cache[i-4];
            cache[i] = v;
        }
    }
    next_idx = n + 1;
}

seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
