/*
 * A296550  Number of n X 4 0..1 arrays with each 1 horizontally, vertically
 *          or antidiagonally adjacent to 3 or 6 neighboring 1s.
 * https://oeis.org/A296550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-4) + a(n-7), with a(1) = 1, a(2) = 1, a(3)
 *            = 3, a(4) = 5, a(5) = 7, a(6) = 10, a(7) = 17; (OEIS A296550)
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 49, u64 n <= 98, u128 n <= 195,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* 1‑based cache */
static size_t cache_cap = 0;  /* current allocated size */

/* Helper: ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache_cap(idx_t n)
{
    if (cache_cap >= (size_t)n + 1) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();  /* out of memory */
    /* initialize newly allocated part to zero */
    for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = (seq_t)0;
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n < 1 */
    ensure_cache_cap(n);

    /* base values for 1..7 */
    static const seq_t base[8] = {0, 1, 1, 3, 5, 7, 10, 17};

    for (idx_t i = 1; i <= n; ++i)
    {
        if (cache[i] != (seq_t)0) continue;  /* already computed */

        if (i <= 7)
        {
            cache[i] = base[i];
        }
        else
        {
            /* recurrence: a(i) = a(i-1) + 2*a(i-4) + a(i-7) */
            seq_t val = cache[i - 1];
            val += ((seq_t)2 * cache[i - 4]);
            val += cache[i - 7];
            cache[i] = val;
        }
    }
    return cache[n];
}
