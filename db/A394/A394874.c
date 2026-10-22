/*
 * A394874  G.f. A(x) satisfies A(x) = 1 - x - x * d/dx log(1 - x*A(x)).
 * https://oeis.org/A394874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A394874
 *  a(0) = 1, a(1) = 0
 *  a(n) = n*a(n-1) + a(n-2) + Σ_{k=1}^{n-1} a(k)*a(n-1-k),  n ≥ 2
 */
#include <stdlib.h>

/* Runtime cache for computed terms */
static seq_t *cache = NULL;      /* array of computed a(n) */
static size_t cache_cap = 0;     /* number of entries allocated */

/* grow the cache to accommodate at least n+1 terms, initialise 0/1 */
static void ensure_cache_size(idx_t n)
{
    if (cache_cap > (size_t)n) return;      /* already large enough */
    size_t new_cap = cache_cap ? cache_cap << 1 : 4;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                      /* out of memory */
    /* initialise new space to zero */
    for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
    cache = tmp;
    cache_cap = new_cap;
    /* initialise the first two terms if this is the first allocation */
    if (cache[0] == (seq_t)0) cache[0] = (seq_t)1;
    if (cache[1] == (seq_t)0) cache[1] = (seq_t)0;
}

/* compute A394874 term a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* allocate cache if necessary */
    ensure_cache_size(n);

    /* if already computed, return cached result */
    if (cache[n] != (seq_t)0) return cache[n];

    /* compute sequentially from 2 up to n */
    for (idx_t i = 2; i <= n; ++i)
    {
        if (cache[i] != (seq_t)0) continue;   /* skip already known */

        seq_t sum = (seq_t)0;
        /* convolution sum Σ_{k=1}^{i-1} a(k)*a(i-1-k) */
        for (idx_t k = 1; k < i; ++k)
        {
            sum += cache[k] * cache[i-1-k];
        }
        cache[i] = ((seq_t)i * cache[i-1]) + cache[i-2] + sum;
    }
    return cache[n];
}
