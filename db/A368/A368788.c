/*
 * A368788  a(n) = (n+1) * (n!)^3 * Sum_{k=1..n} 1/((k+1) * (k!)^3).
 * https://oeis.org/A368788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A368788: a(0) = 0; a(n) = (n+1)*n^2*a(n-1)+1 (n≥1) */

#include <stdlib.h>

/*
 * Compute a(n) using the recurrence.
 *
 * The function caches previously computed terms to support arbitrary
 * order of calls.  It only uses +, -, *, /, %, and the usual
 * integer operators required by the rules.
 *
 * Internally the cache grows as needed.  For built‐in integer types
 * this behaves identically to the arbitrary‐precision build.
 */
seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_sz = 0;   /* number of elements stored */

    /* initialise cache with a(0) = 0 if empty */
    if (cache_sz == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* fallback on failure */
        cache[0] = (seq_t)0;
        cache_sz = 1;
    }

    /* if n already cached, return it */
    if (n < cache_sz)
        return cache[n];

    /* extend cache up to n */
    idx_t new_sz = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    if (!tmp) return (seq_t)0;           /* fallback on failure */
    cache = tmp;

    /* compute missing terms */
    for (idx_t i = cache_sz; i <= n; ++i) {
        seq_t i_seq  = (seq_t)i;
        seq_t i1_seq = (seq_t)(i + 1);
        seq_t prev   = cache[i - 1];

        /* (i+1) * i^2 * prev + 1 */
        seq_t mul = i_seq * i_seq;     /* i^2 */
        mul *= i1_seq;                /* * (i+1) */
        mul *= prev;                  /* * a(i-1) */
        mul += (seq_t)1;

        cache[i] = mul;
    }

    cache_sz = new_sz;
    return cache[n];
}
