/*
 * A368342  Sum of digits of the numbers 0..n-1 in factorial base (A108731).
 * https://oeis.org/A368342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{i=0}^{n-1} A034968(i)
 * Range:     u8 n <= 67, u16 n <= 6455, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (sum of the factorial‑based digits of 0..n‑1) */

#include <stddef.h>   /* NULL, size_t */
#include <stdlib.h>   /* malloc, realloc */
#include "../utils/digit_sum_factorial.h"

static seq_t *cache = NULL;       /* cache[i] = a(i) for i >= 0 */
static idx_t cache_limit = 0;     /* largest index stored in cache */

seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));  /* holds a(0) */
        if (cache == NULL) return 0;             /* allocation failed */
        cache[0] = 0;
        cache_limit = 0;
    }

    /* Return the stored value if already computed */
    if (n <= cache_limit)
        return cache[n];

    /* Grow the cache to hold indices 0..n */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL) return 0;        /* allocation failed */
    cache = tmp;

    /* Compute new entries incrementally.
       a(i) = a(i-1) + digit_sum_factorial(i-1)   for i > cache_limit */
    for (idx_t i = cache_limit + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] + digit_sum_factorial(i - 1);
    }
    cache_limit = n;
    return cache[n];
}
