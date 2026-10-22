/*
 * A343752  a(1) = 1; for n > 1, a(n) = n if a(n-1) is divisible by n,
 *          otherwise a(n) = a(n-1)+n.
 * https://oeis.org/A343752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = n if a(n‑1) % n == 0, otherwise
 *            a(n‑1) + n
 * Range:     u8 n <= 41, u16 n <= 1324, u32 n <= 9839, u64 n <= 9839,
 *            u128 n <= 9839, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper for reallocation that works with arbitrary‑precision seq_t types. */
#include "../utils/seq_realloc.h"

/* Memoization cache for computed terms. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity in elements */
static idx_t  cache_len = 0;   /* number of valid terms stored */

/* Grow the cache so that it can hold at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < needed)
            new_cap <<= 1;               /* ensure new_cap >= needed */

        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp) abort();              /* out of memory */

        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Compute the n-th term of sequence A343752. */
seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined for offset 1, but safe */
        return (seq_t)0;

    /* Ensure the cache can store term n. */
    grow_cache((size_t)n);

    /* Initialize cache[0] on first call. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Return cached value if already computed. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t prev = cache[i - 2];            /* a(i-1)  */
        seq_t mod  = prev % (seq_t)i;         /* a(i-1) mod i */
        if (mod == (seq_t)0)
            cache[i - 1] = (seq_t)i;           /* a(i) = i        */
        else
            cache[i - 1] = prev + (seq_t)i;    /* a(i) = a(i‑1)+i */
    }
    cache_len = n;
    return cache[n - 1];
}
