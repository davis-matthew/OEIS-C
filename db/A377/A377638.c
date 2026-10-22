/*
 * A377638  Number of edge cuts in the n-gear graph.
 * https://oeis.org/A377638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1)-55*a(n-2)+58*a(n-3)-16*a(n-4) with initial
 *            values a(0)=1, a(1)=5, a(2)=45, a(3)=419
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;            /* memoised values */
static size_t cache_len = 0;           /* number of cached terms */

static void ensure_cache(idx_t n)
{
    /* already computed? */
    if ((size_t)n + 1 <= cache_len)
        return;

    size_t old_len = cache_len;
    size_t new_len = (size_t)n + 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)        /* allocation failed – abort silently */
        return;
    cache = tmp;
    cache_len = new_len;

    /* initialise new entries to 0 */
    for (size_t i = old_len; i < new_len; ++i)
        cache[i] = 0;

    /* set the initial values (they are needed for the recurrence) */
    if (new_len > 0) cache[0] = 1;
    if (new_len > 1) cache[1] = 5;
    if (new_len > 2) cache[2] = 45;
    if (new_len > 3) cache[3] = 419;

    /* compute any new terms using the recurrence */
    size_t start = (old_len > 4) ? old_len : 4;
    for (size_t i = start; i <= (size_t)n; ++i)
    {
        cache[i] = 14 * cache[i-1];
        cache[i] -= 55 * cache[i-2];
        cache[i] += 58 * cache[i-3];
        cache[i] -= 16 * cache[i-4];
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
