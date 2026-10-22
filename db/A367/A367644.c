/*
 * A367644  Number of maximal independent vertex sets and minimal vertex
 *          covers in the n-trapezohedral graph.
 * https://oeis.org/A367644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - a(n-4) Initial values:
 *            a(1)=3, a(2)=3, a(3)=6, a(4)=11
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for abort(), realloc() */

static seq_t *cache = NULL;      /* 1‑indexed cache, index 0 unused */
static idx_t  cache_max = 0;     /* highest allocated index */
static idx_t  cache_done = 0;    /* highest index for which a(i) is known */

/* Ensure the cache can hold indices 1 … need. */
static void cache_expand(idx_t need)
{
    if (need <= cache_max)
        return;
    seq_t *newp = (seq_t *)realloc(cache, sizeof(seq_t) * (need + 1));
    if (!newp)
        abort();                    /* out of memory */
    cache = newp;
    cache_max = need;
}

/* Initialise the base cases once. */
static void init_cache(void)
{
    if (cache_done >= 4)
        return;
    cache_expand(4);                /* make sure we have room for indices 1…4 */
    cache[1] = (seq_t)3;
    cache[2] = (seq_t)3;
    cache[3] = (seq_t)6;
    cache[4] = (seq_t)11;
    cache_done = 4;
}

/* Compute a(n) for any n ≥ 1 in any order of calls. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* undefined for n < 1 */

    cache_expand(n);                /* ensure storage up to n */
    init_cache();                   /* set base values once */

    if (n <= cache_done)
        return cache[n];

    for (idx_t i = cache_done + 1; i <= n; ++i) {
        seq_t term = cache[i - 1] * (seq_t)3;
        term -= cache[i - 2] * (seq_t)3;
        term += cache[i - 3] * (seq_t)2;
        term -= cache[i - 4];
        cache[i] = term;
    }
    cache_done = n;
    return cache[n];
}
