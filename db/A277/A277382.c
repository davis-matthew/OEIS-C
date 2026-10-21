/*
 * A277382  a(n) = n!*LaguerreL(n, -3).
 * https://oeis.org/A277382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*LaguerreL(n, -3); recurrence: a(0)=1, a(1)=4,
 *            a(n)=2*(n+1)*a(n-1)-(n-1)^2*a(n-2) (n>=2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for already computed values */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;

/* Ensure cache contains values up to n */
static void ensure_cache(idx_t n)
{
    /* Nothing to do if already ok */
    if (cache_cap > n)
        return;

    /* Allocate or resize cache */
    idx_t new_cap = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed – terminate */
        abort();
    }
    cache = new_cache;

    /* Initialize base values if they don't exist yet */
    if (cache_cap == 0) {
        cache[0] = (seq_t)1;
        if (new_cap > 1)
            cache[1] = (seq_t)4;
    } else if (cache_cap == 1) {
        /* cache[0] already set, just set a(1) */
        cache[1] = (seq_t)4;
    }

    /* Compute missing terms using the recurrence */
    idx_t start = cache_cap < 2 ? 2 : cache_cap;
    for (idx_t i = start; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        seq_t bi1  = (seq_t)(i + 1);            /* (n+1)   */
        seq_t bm1  = (seq_t)(i - 1);            /* (n-1)   */
        bm1 = bm1 * bm1;                        /* (n-1)^2 */
        seq_t term = 2 * bi1 * prev1 - bm1 * prev2;
        cache[i] = term;
    }

    cache_cap = new_cap;
}

/* Public function: returns a(n) */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
