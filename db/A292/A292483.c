/*
 * A292483  p-INVERT of the odd positive integers, where p(S) = (1 - S)^3.
 * https://oeis.org/A292483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=3, a(1)=15, a(2)=61, a(3)=240, a(4)=912, a(5)=3376 a(n) =
 *            9*a(n-1) - 27*a(n-2) + 27*a(n-3) for n >= 6 The function caches
 *            already-computed values so that repeated calls in arbitrary
 *            order remain fast
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 35, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* cached sequence */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initial values */
    static const seq_t base[6] = { (seq_t)3, (seq_t)15, (seq_t)61,
                                   (seq_t)240, (seq_t)912, (seq_t)3376 };

    /* initialise cache if first call */
    if (cache_len == 0) {
        cache_len = 6;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) { /* allocation failed – return 0 as sentinel */
            return (seq_t)0;
        }
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = base[i];
    }

    /* if already computed, return */
    if (n < cache_len)
        return cache[n];

    /* extend cache to include term n */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {
        /* reallocation failed – keep old cache and return 0 */
        return (seq_t)0;
    }
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = ((seq_t)9   * cache[i-1]) -
                   ((seq_t)27  * cache[i-2]) +
                   ((seq_t)27  * cache[i-3]);
    }

    cache_len = new_len;
    return cache[n];
}
