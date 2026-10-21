/*
 * A275788  a(0) = 0, a(n+1) = 2*a(n) + (-1)^floor(n/3).
 * https://oeis.org/A275788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n+1) = 2*a(n) + (-1)^⌊n/3⌋ for n ≥ 0 (OEIS A275788)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t len   = 0;   /* number of computed terms (highest index + 1) */

    /* initialise cache with a(0) */
    if (cache == NULL)
    {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failed */
        cache[0] = (seq_t)0;
        len = 1;
    }

    /* already computed? */
    if (n < len) return cache[n];

    /* expand cache to index n */
    seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) return (seq_t)0;
    cache = tmp;

    /* compute missing terms */
    for (idx_t i = len; i <= n; ++i)
    {
        seq_t prev = cache[i - 1];
        /* sign: +1 when ⌊(i-1)/3⌋ is even, -1 otherwise */
        if ( ((i - 1) / 3) & 1 )
            cache[i] = prev * 2 - (seq_t)1;
        else
            cache[i] = prev * 2 + (seq_t)1;
    }

    len = n + 1;
    return cache[n];
}
