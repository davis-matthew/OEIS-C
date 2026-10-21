/*
 * A259738  Number of (n+2) X (4+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00010101 or 01010101.
 * https://oeis.org/A259738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 22, u64 n <= 48, u128 n <= 101,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A259738
   Recurrence (empirical, verified up to many terms):
        a(n) = a(n‑1) + 4·a(n‑3) + 5·a(n‑4) + 3·a(n‑5) +
               5·a(n‑6) + a(n‑7) – 4·a(n‑8) – a(n‑9) + a(n‑10)
   for n > 11, with initial values
        a(1)=62, a(2)=136, a(3)=374, a(4)=868, a(5)=1882,
        a(6)=4456, a(7)=10570, a(8)=24150, a(9)=55686,
        a(10)=130032, a(11)=301564.
   The function memoises results so it works correctly for any order
   of calls.  It works for both fixed‑width integers and arbitrary‑precision
   integers supplied by the build.  All bounds checks and casts follow
   the restrictions in the problem statement.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

#define NUM_INIT 11

/* Initial known terms (1‑based index). */
static const seq_t init[NUM_INIT] = {
    (seq_t)62, (seq_t)136, (seq_t)374, (seq_t)868, (seq_t)1882,
    (seq_t)4456, (seq_t)10570, (seq_t)24150, (seq_t)55686,
    (seq_t)130032, (seq_t)301564
};

/* Memoisation cache (index 0 unused). */
static seq_t *cache      = NULL; /* points to aligned array of seq_t */
static size_t cache_cap  = 0;     /* number of elements allocated */
static size_t cache_last = 0;     /* largest index that has been evaluated */

/* Extend the cache to hold at least 'new_cap' elements (indices 0 .. new_cap-1). */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap) return;

    seq_t *new_cache = alloc_seq(new_cap);
    if (!new_cache) exit(1);

    /* Preserve existing values if any. */
    if (cache)
    {
        for (size_t i = 0; i <= cache_last; ++i) new_cache[i] = cache[i];
        free_seq(cache);
    }
    else
    {
        /* On first allocation set element 0 to zero (unused but safe). */
        new_cache[0] = (seq_t)0;
    }

    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Public function – compute a(n) for any n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* behaviour below offset is irrelevant */

    /* Work with native size_t for indices. */
    size_t idx = (size_t)n;

    /* Ensure we have enough space in the cache (indices 0..idx). */
    ensure_capacity(idx + 1);

    /* If this is the first call, initialise the cache with zero at 0
       (already done in ensure_capacity when cache==NULL) and load the
       supplied initial terms up to NUM_INIT. */
    if (cache_last == 0 && cache[0] == 0)
    {
        /* Load initial terms (ensure we don't go past provided data). */
        size_t to_load = (idx < NUM_INIT) ? idx : NUM_INIT;
        for (size_t i = 1; i <= to_load; ++i) cache[i] = init[i - 1];
        cache_last = to_load;
    }

    /* If the requested value is already cached, return it. */
    if (idx <= cache_last) return cache[idx];

    /* Compute missing values up to idx using the recurrence. */
    for (size_t i = cache_last + 1; i <= idx; ++i)
    {
        if (i <= NUM_INIT)
        {
            cache[i] = init[i - 1];
        }
        else
        {
            cache[i] =
                cache[i - 1]                        /* a(n-1) */
              + (seq_t)4 * cache[i - 3]               /* 4·a(n‑3) */
              + (seq_t)5 * cache[i - 4]               /* 5·a(n‑4) */
              + (seq_t)3 * cache[i - 5]               /* 3·a(n‑5) */
              + (seq_t)5 * cache[i - 6]               /* 5·a(n‑6) */
              +       cache[i - 7]                    /*  a(n‑7) */
              - (seq_t)4 * cache[i - 8]               /* -4·a(n‑8) */
              -       cache[i - 9]                    /* - a(n‑9) */
              +       cache[i -10];                  /* + a(n‑10) */
        }
    }

    cache_last = idx;
    return cache[idx];
}
