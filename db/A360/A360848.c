/*
 * A360848  Number of dominating induced trees in the n-ladder graph P_2_X
 *          P_n.
 * https://oeis.org/A360848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) for n > 4; a(1)=3, a(2)=8, a(3)=17,
 *            a(4)=32
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    /* static cache of computed values (index 0 unused) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* largest index currently stored */

    if (cache_size == 0) {
        /* allocate space for indices 0..4 inclusive */
        cache = (seq_t *)calloc(5, sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed */

        /* set the first four terms */
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)17;
        cache[4] = (seq_t)32;

        cache_size = 4;
    }

    if (n <= cache_size)
        return cache[n];

    /* grow the cache to include index n */
    idx_t new_size = n + 1;          /* +1 because indices start at 0 */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) return (seq_t)0; /* allocation failed */

    cache = new_cache;

    /* compute new values using the recurrence */
    for (idx_t i = cache_size + 1; i <= n; ++i)
    {
        if (i <= 4) {
            /* first four terms are already set */
            continue;
        }
        cache[i] = cache[i - 1] + (seq_t)2 * cache[i - 2];
    }

    cache_size = n;
    return cache[n];
}
