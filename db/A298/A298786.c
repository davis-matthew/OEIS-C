/*
 * A298786  Expansion of (x^4 + 2*x^3 + 4*x^2 + 2*x + 1) / ((1 - x)*(1 -
 *          x^3)).
 * https://oeis.org/A298786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) for n>4 a(0) = 1, a(1) = 3, a(2)
 *            = 7, a(3) = 10, a(4) = 13
 * Range:     u8 n <= 76, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static size_t computed_max = 4;   /* highest index that has been computed */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_size = 5;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failure – not expected */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)10;
        cache[4] = (seq_t)13;
    }

    /* grow allocation if necessary */
    size_t required = (size_t)n + 1;
    if (required > cache_size) {
        size_t new_size = required > cache_size * 2 ? required : cache_size * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure – not expected */
        cache      = tmp;
        cache_size = new_size;
    }

    /* compute missing terms using recurrence */
    if ((size_t)n > computed_max) {
        for (size_t i = computed_max + 1; i <= (size_t)n; ++i) {
            cache[i] = cache[i-1] + cache[i-3] - cache[i-4];
        }
        computed_max = (size_t)n;
    }

    return cache[n];
}
