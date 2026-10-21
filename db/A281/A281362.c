/*
 * A281362  a(0) = 1, a(1) = 2; for n>1, a(n) = a(n-1) + a(n-2) + floor(n/2).
 * https://oeis.org/A281362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 2; for n > 1, a(n) = a(n‑1) + a(n‑2) +
 *            floor(n/2)
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Initialize cache with the two seed values. */
    if (cache_size == 0) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
    }

    /* Grow and fill the cache up to index n if necessary. */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!cache) abort();
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t prev1 = cache[i - 1];
            seq_t prev2 = cache[i - 2];
            seq_t half  = (seq_t)i / 2;          /* floor(i/2) */
            cache[i] = prev1 + prev2 + half;     /* recurrence */
        }
        cache_size = new_size;
    }

    return cache[n];
}
