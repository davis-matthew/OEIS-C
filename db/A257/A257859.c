/*
 * A257859  a(n) = (2*n-1)*a(n-1) - a(n-2) with a(0)=2, a(1)=1.
 * https://oeis.org/A257859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = (2*n-1)*a(n-1) - a(n-2); a(0)=2, a(1)=1 */
#include <stdlib.h>

static seq_t *cache = NULL;     /* previously computed values, length = cache_len */
static size_t cache_len = 0;    /* number of valid entries in cache */

seq_t a(idx_t n)
{
    /* Handle the trivial base cases directly.  No allocation needed. */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)1;

    /* If we already have the requested term cached, return it. */
    if (cache && cache_len > (size_t)n) return cache[(size_t)n];

    size_t needed = (size_t)n + 1;   /* length we must have in the cache */
    size_t old_len = cache_len;      /* remember the previous cache size */

    /* Resize the cache if necessary.  After reallocation we initialise
       the base values, which are constant. */
    if (cache_len < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();          /* allocation error – terminate */
        cache = tmp;
        if (needed > 0) cache[0] = (seq_t)2;
        if (needed > 1) cache[1] = (seq_t)1;
        cache_len = needed;
    }

    /* Compute any missing terms, starting where the cache ended. */
    size_t start = (old_len > 2) ? old_len : 2;
    for (size_t k = start; k <= (size_t)n; ++k) {
        seq_t prev   = cache[k - 1];
        seq_t prev2  = cache[k - 2];
        seq_t mult   = (seq_t)(2 * k - 1);
        cache[k] = mult * prev - prev2;
    }

    return cache[(size_t)n];
}
