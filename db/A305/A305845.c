/*
 * A305845  a(1) = a(2) = a(3) = 1; for n > 3, a(n) = a(a(n-2)) + a(n-a(n-2)).
 * https://oeis.org/A305845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A305845
 *  a(1) = a(2) = a(3) = 1;
 *  a(n) = a(a(n-2)) + a(n - a(n-2))   for n > 3
 *
 *  This implementation memoises all values up to the largest
 *  requested index so that arbitrary‑order calls are O(1) per new
 *  term.  The cache grows dynamically; realloc is cast to match
 *  the type seq_t, which may be an arbitrary‑precision integer
 *  class in C++.
 */

#include <stdlib.h>   /* realloc, size_t */

static seq_t *cache = NULL;        /* 1‑based cache of already computed terms */
static size_t cache_cap = 0;       /* current allocated size (>=1 for any used index) */
static idx_t  computed_up_to = 0;  /* largest n for which cache[n] is valid */

/* Ensure the cache can hold indices up to 'need' (inclusive). */
static void grow_cache(idx_t need)
{
    if ((size_t)need < cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 128;
    while (new_cap <= (size_t)need)
        new_cap <<= 1;

    /* Reallocate and zero any newly added slots. */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) /* allocation failure – terminate program */
        abort();

    /* Zero the fresh part of the array. */
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = 0;

    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* sequence is defined only for n ≥ 1 */

    /* If the requested term is not yet available, compute all missing ones. */
    if (n > computed_up_to)
    {
        grow_cache(n);

        for (idx_t i = computed_up_to + 1; i <= n; ++i)
        {
            if (i <= 3)
            {
                cache[(size_t)i] = (seq_t)1;
            }
            else
            {
                /* Retrieve a(i‑2) and use it as an index.  Cast is safe
                 * because the sequence is monotone and a(i‑2) ≤ i‑2 ≤ n. */
                idx_t idx_a_i_2 = (idx_t)cache[(size_t)(i - 2)];
                seq_t val1 = cache[(size_t)idx_a_i_2];
                seq_t val2 = cache[(size_t)(i - idx_a_i_2)];
                cache[(size_t)i] = val1 + val2;
            }
        }
        computed_up_to = n;
    }

    return cache[(size_t)n];
}
