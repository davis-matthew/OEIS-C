/*
 * A293720  Expansion of e.g.f.: exp(x + 4*x^2).
 * https://oeis.org/A293720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence a(n+2) = a(n+1) + 8*(n+1)*a(n), a(0)=1, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 20, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache to store previously computed terms.
   cache_len  : number of terms currently stored (≥ 2 after first init).
   cache_cap  : allocated capacity of cache array. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* Ensure cache is initialized with a(0) and a(1). */
    if (cache_len == 0)
    {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();          /* allocation failure: terminate */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* If requested index is already computed, return it immediately. */
    if (n < cache_len)
        return cache[n];

    /* Grow cache capacity if necessary. */
    if (n + 1 > cache_cap)
    {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= n) new_cap <<= 1;   /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                    /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        /* next = a(i-1) + 8*(i-1)*a(i-2)   because (i-2)+1 = i-1 */
        seq_t factor = (seq_t)(8 * (i - 1));
        seq_t next = cache[i - 1] + factor * cache[i - 2];
        cache[i] = next;
    }
    cache_len = n + 1;

    return cache[n];
}
