/*
 * A375425  Expansion of e.g.f. exp(-x * (1 - x)) / (1 - x)^3.
 * https://oeis.org/A375425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*a(n-1) + 3*(n-1)*a(n-2) - 2*(n-1)*(n-2)*a(n-3) a(0)
 *            = 1, a(1) = 2, a(2) = 9
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static idx_t cached_max = 0;   /* highest index already computed (initially 2 after init) */

seq_t a(idx_t n)
{
    /* Initialise cache and base values on first call or when cache is NULL */
    if (cache == NULL) {
        size_t init_cap = (size_t)(n + 1);
        if (init_cap < 3) init_cap = 3;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        cache_capacity = init_cap;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)9;
        cached_max = 2;
    }

    /* Return cached value if already computed */
    if (n <= cached_max) {
        return cache[n];
    }

    /* Grow cache if necessary */
    size_t needed_cap = (size_t)(n + 1);
    if (needed_cap > cache_capacity) {
        cache = (seq_t *)realloc(cache, needed_cap * sizeof(seq_t));
        cache_capacity = needed_cap;
    }

    /* Compute missing terms via the recurrence */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t term1 = cache[i - 1] * ((seq_t)(i + 1));
        seq_t term2 = cache[i - 2] * ((seq_t)(3 * (i - 1)));
        seq_t term3 = cache[i - 3] * ((seq_t)(2 * (i - 1) * (i - 2)));
        cache[i] = term1 + term2 - term3;
    }
    cached_max = n;
    return cache[n];
}
