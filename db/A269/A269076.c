/*
 * A269076  Number of 2 X n binary arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two not more than once.
 * https://oeis.org/A269076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 5*a(n-2) - 6*a(n-3) - 9*a(n-4) Initial values:
 *            a(1)=4, a(2)=11, a(3)=27, a(4)=76
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 49, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

/* Ensure the cache can hold indices up to n (1‑based). */
static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap) return;
    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);   /* out of memory */
    /* Zero out newly allocated portion for safety */
    for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;
    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute a(n) with memoisation. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* behaviour below offset not required */
    ensure_cache(n);
    if (cache_len >= n) return cache[n];

    /* Compute values from (cache_len+1) up to n */
    for (idx_t i = (cache_len == 0 ? 1 : cache_len + 1); i <= n; ++i)
    {
        if (i == 1)          cache[i] = (seq_t)4;
        else if (i == 2)     cache[i] = (seq_t)11;
        else if (i == 3)     cache[i] = (seq_t)27;
        else if (i == 4)     cache[i] = (seq_t)76;
        else
        {
            seq_t term   = cache[i-1] * (seq_t)2 + cache[i-2] * (seq_t)5;
            seq_t sub    = cache[i-3] * (seq_t)6 + cache[i-4] * (seq_t)9;
            cache[i] = term - sub;
        }
    }
    cache_len = n;
    return cache[n];
}
