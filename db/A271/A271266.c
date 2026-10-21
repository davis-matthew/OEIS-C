/*
 * A271266  a(n) = Product_{k=1..n} (k^2 + 21).
 * https://oeis.org/A271266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} (k² + 21)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_n = 0;
    static size_t cache_cap = 0;

    /* Initialise cache on first call */
    if (!cache) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;           /* allocation failure */
        cache[0] = (seq_t)1;            /* a(0) = 1 */
        cache_n = 0;
    }

    /* Return previously computed value */
    if (n <= cache_n)
        return cache[n];

    /* Grow storage if required */
    size_t needed = (size_t)(n + 1);
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Incrementally compute missing terms */
    for (idx_t i = cache_n + 1; i <= n; ++i) {
        seq_t term = (seq_t)i * (seq_t)i + (seq_t)21;   /* k² + 21 */
        cache[i] = cache[i - 1] * term;                 /* a(i) = a(i-1) * term */
    }
    cache_n = n;
    return cache[n];
}
