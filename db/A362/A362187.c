/*
 * A362187  a(n) = (n^2 - n)!.
 * https://oeis.org/A362187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 - n)! (offset 0)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   seq_from_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_from_uint64.h"

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t max_computed = 0;

static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap) return;
    idx_t new_cap = (n + 1) * 2;
    /* Interpret realloc result as a pointer to seq_t.  Works in C++ with
       reinterpret_cast and in C with a C‑style cast. */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Special cases: a(0) = a(1) = 1 */
    if (n == 0 || n == 1)
        return seq_from_uint64(1);

    /* Initialise cache lazily. */
    if (!cache)
    {
        ensure_cache(n);
        cache[0] = seq_from_uint64(1);
        cache[1] = seq_from_uint64(1);
        max_computed = 1;
    }
    else
    {
        ensure_cache(n);
    }

    /* If already computed, simply return. */
    if (n <= max_computed)
        return cache[n];

    /* Compute missing values up to n using the recurrence
       a(i) = a(i-1) * Π_{k=(i-1)^2-(i-1)+1}^{i^2-i} k   (2i-2 terms). */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        idx_t start = ((i - 1) * (i - 1) - (i - 1)) + 1;   /* (i-1)^2-(i-1)+1 */
        idx_t end   = i * i - i;                           /* i^2-i          */
        cache[i] = cache[i - 1];
        for (idx_t k = start; k <= end; ++k)
            cache[i] *= seq_from_uint64(k);
    }

    max_computed = n;
    return cache[n];
}
