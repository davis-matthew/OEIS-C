/*
 * A370175  a(n) = floor(x*a(n-1)) for n > 0 where x = (5+3*sqrt(5))/2, a(0) =
 *          1.
 * https://oeis.org/A370175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(((5+3*sqrt(5))/2)*a(n-1)) Recurrence: a(0)=1,
 *            a(1)=5, a(2)=29, a(n)=6*a(n-1)-5*a(n-3) for n≥3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cached values a[0]..a[cache_size-1] */
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* handle first three values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)29;

    /* initialize cache on first call */
    if (cache_size == 0) {
        /* Allocate space for a[0]..a[2] */
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)29;
        cache_size = 3;
    }

    /* if requested value is already cached, return it */
    if (n < cache_size) return cache[n];

    /* grow the cache to hold required index */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failed; return 0 as a sentinel (normally program aborts) */
        return (seq_t)0;
    }
    cache = new_cache;

    /* compute missing terms using recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* a(i) = 6*a(i-1) - 5*a(i-3) */
        seq_t val = 6 * cache[i - 1] - 5 * cache[i - 3];
        cache[i] = val;
    }

    cache_size = n + 1;   /* update cached size */
    return cache[n];
}
