/*
 * A342728  a(n) is the least number k such that A066323(k) = n.
 * https://oeis.org/A342728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 49, u64 n <= 97, u128 n <= 193,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n for n <= 7;
 * a(n) = a(n-1) + 16 * a(n-6) - 16 * a(n-7) for n > 7.
 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements currently stored */

    /* If the requested term is already cached, return it. */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache to hold all terms up to n. */
    size_t new_len = (size_t)n + 1;   /* elements needed (0..n) */

    if (cache == NULL) {
        cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (cache == NULL) abort();
    } else if (new_len > (size_t)cache_len) {
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (cache == NULL) abort();
    }

    /* Compute terms from the first missing index up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 7) {
            cache[i] = (seq_t)i;
        } else {
            cache[i] = cache[i-1]
                     + (seq_t)16 * cache[i-6]
                     - (seq_t)16 * cache[i-7];
        }
    }

    cache_len = new_len;
    return cache[n];
}
