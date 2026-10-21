/*
 * A295685  a(n) = a(n-1) + a(n-3) + a(n-4), where a(0) = 2, a(1) = 1, a(2) =
 *          1, a(3) = 1.
 * https://oeis.org/A295685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4), with a(0)=2, a(1)=1, a(2)=1,
 *            a(3)=1
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* cache of computed terms */
static idx_t max_cached = 0;  /* one past the last cached index */

/* Compute a(n) using memoization */
seq_t a(idx_t n)
{
    /* initialize the cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (cache == NULL)
            return 0;                  /* allocation failure: return 0 */
        cache[0] = 2;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 1;
        max_cached = 4;
    }

    /* if the requested term is already cached, return it */
    if (n < max_cached)
        return cache[n];

    /* grow the cache to accommodate up to n */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (new_cache == NULL)
        return 0;                      /* allocation failure: return 0 */
    cache = new_cache;

    /* compute missing terms sequentially */
    for (idx_t i = max_cached; i <= n; ++i) {
        if (i == 0)
            cache[i] = 2;
        else if (i == 1 || i == 2 || i == 3)
            cache[i] = 1;
        else
            cache[i] = cache[i - 1] + cache[i - 3] + cache[i - 4];
    }

    max_cached = n + 1;
    return cache[n];
}
