/*
 * A381504  Expansion of e.g.f. exp(-x/4) / (1-4*x)^(1/16).
 * https://oeis.org/A381504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=0, for n>1: a(n) = (n-1) * (4*a(n-1) + a(n-2))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;          /* array of computed a(i) */
static size_t cache_size = 0;        /* current allocation size */
static idx_t last_computed = 0;      /* highest index for which a(i) is set */

/* ensure the cache holds at least new_size elements */
static void extend_cache(size_t new_size)
{
    if (cache_size < new_size) {
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* fatal error: out of memory */
        cache = tmp;
        cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    /* make sure the cache is at least large enough for indices 0..n */
    size_t needed = (size_t)n + 1;
    if (needed < 2) needed = 2;                 /* need space for a(0) and a(1) */
    extend_cache(needed);

    /* initialise base values once */
    if (last_computed == 0) {
        cache[0] = (seq_t)1;                    /* a(0) */
        cache[1] = (seq_t)0;                    /* a(1) */
        last_computed = 1;
    }

    if (n <= last_computed)                    /* value already known */
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        seq_t term = ((seq_t)(i - 1)) * ( ((seq_t)4 * prev1) + prev2 );
        cache[i] = term;
    }
    last_computed = n;
    return cache[n];
}
