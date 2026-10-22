/*
 * A365608  Number of degree 4 vertices in the n-Sierpinski carpet graph.
 * https://oeis.org/A365608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0, a(2)=4, a(3)=100
   a(n)=12*a(n-1)-35*a(n-2)+24*a(n-3)  for n>3 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* stores computed terms; index 0 unused */
static size_t cache_size = 0;         /* allocated size of cache array */
static idx_t  cached_max = 0;         /* largest n whose value is stored */

/* Compute the n‑th term of OEIS A365608 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* outside the defined offset */

    /* Ensure the cache is large enough to hold terms up to n */
    size_t needed = (size_t)n + 1;     /* include index 0 */
    if (cache == NULL) {
        /* initialise the cache with enough space for the first three terms */
        cache_size = needed > 4 ? needed : 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;           /* allocation failure */

        /* base values */
        cache[0] = (seq_t)0;     /* unused */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)100;

        cached_max = 3;
    } else if (needed > cache_size) {
        /* grow the cache to accommodate the requested index */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;       /* allocation failure */

        cache = tmp;
        cache_size = needed;
    }

    /* Compute any missing terms using the recurrence */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        /* For i==1,2,3 the base values are already set */
        if (i >= 4) {
            cache[i] =
                12 * cache[i - 1]
              - 35 * cache[i - 2]
              + 24 * cache[i - 3];
        }
    }
    cached_max = n;

    return cache[n];
}
