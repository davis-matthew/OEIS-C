/*
 * A371209  Number of ordered factorizations of n into factors > 2.
 * https://oeis.org/A371209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 719, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A371209: Number of ordered factorizations of n into factors > 2
   Formula: a(1) = 1; a(n) = Σ_{d|n, n/d > 2} a(d)   (n ≥ 2) */

/* Include utilities for dynamic array allocation and deallocation. */
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_alloc_new_array.h"   /* seq_alloc_new_array(size_t) -> seq_t*   */
#include "../utils/seq_free_array.h"        /* seq_free_array(seq_t*)                 */

static seq_t *cache = NULL;      /* cache[0] unused, cache[1] = a(1) */
static size_t cache_cap = 0;     /* number of elements allocated (>= index+1) */

/* Recompute the whole cache up to index n (inclusive).  Complexity O(n log n). */
static void ensure_cache(idx_t n)
{
    if (n < 1) return;

    /* Allocate a new array with enough space for indices 0..n. */
    seq_t *new_cache = seq_alloc_new_array((size_t)n + 1);
    if (!new_cache) abort(); /* Out of memory */

    /* Zero all entries – works for integral as well as arbitrary‑precision types. */
    memset(new_cache, 0, ((size_t)n + 1) * sizeof(seq_t));

    /* Base case. */
    new_cache[1] = (seq_t)1;   /* a(1) = 1 */

    /* For each divisor d, distribute its contribution a(d) to all multiples
       n = d*k with k ≥ 3 (i.e. n/d ≥ 3). */
    for (idx_t d = 1; d <= n; ++d)
    {
        seq_t val = new_cache[d];
        if (val == 0) continue;          /* d contributes nothing */

        idx_t m = d * 3;                  /* first multiple with factor ≥ 3 */
        while (m <= n)
        {
            new_cache[m] += val;
            m += d;
        }
    }

    /* Free the previous cache and replace it. */
    if (cache) seq_free_array(cache);
    cache     = new_cache;
    cache_cap = (size_t)n + 1;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* a(0) is undefined; return 0 for safety */
    if (cache_cap <= (size_t)n) ensure_cache(n);
    return cache[n];
}
