/*
 * A280154  a(n) = 5*Lucas(n).
 * https://oeis.org/A280154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*Lucas(n); recurrence: a(0) = 10, a(1) = 5, a(n) =
 *            a(n‑1) + a(n‑2) (n>1)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t   cache_size = 0;

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)10;
    if (n == 1) return (seq_t)5;

    /* Ensure the cache is initialised with the first two terms. */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return 0;          /* out‑of‑memory fallback */
        cache[0] = (seq_t)10;
        cache[1] = (seq_t)5;
    }

    /* Grow the cache if necessary. */
    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) return 0;      /* out‑of‑memory fallback */
        cache = new_cache;
        for (idx_t i = cache_size; i <= n; ++i) {
            /* a(i) = a(i-1) + a(i-2) */
            cache[i] = cache[i - 1] + cache[i - 2];
        }
        cache_size = new_size;
    }

    return cache[n];
}
