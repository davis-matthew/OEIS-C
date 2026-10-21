/*
 * A291066  Number of edges in the n-Menger sponge graph.
 * https://oeis.org/A291066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 28*a(n-1) - 160*a(n-2) (n ≥ 3) a(1) = 24 a(2) = 672
 *            (valid for n ≥ 1, undefined for n = 0)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for arbitrary‑order calls */
    static seq_t *cache = NULL;     /* array of computed a(k), 1 ≤ k ≤ cache_len */
    static idx_t cache_len = 0;     /* highest k for which cache[k] is valid */

    if (n == 0)                /* offset guard */
        return (seq_t)0;

    /* grow cache if needed */
    if (n > cache_len) {
        /* allocate/resize storage up to index n */
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!cache)           // allocation failed: fall back to zero
            return (seq_t)0;

        /* compute missing terms */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)24;
            } else if (i == 2) {
                cache[i] = (seq_t)672;
            } else {
                /* recurrence a(i) = 28*a(i-1) - 160*a(i-2) */
                cache[i] = (seq_t)28 * cache[i - 1] - (seq_t)160 * cache[i - 2];
            }
        }
        cache_len = n;
    }

    return cache[n];
}
