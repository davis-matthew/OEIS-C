/*
 * A277492  Number of vertices in the metrically regular triangulation of the
 *          n-th approximation of the Koch snowflake fractal.
 * https://oeis.org/A277492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=3, a(1)=13, a(2)=96, a(n)=13*a(n-1)-36*a(n-2) for n>2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialize the cache for the first three terms. */
    if (cache_size == 0) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – not expected in tests */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)13;
        cache[2] = (seq_t)96;
    }

    /* Fast return if the requested term is already computed. */
    if (n < cache_size) return cache[n];

    /* Grow the cache to contain indices up to n. */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!new_cache) return 0;          /* allocation failure – not expected */
    cache = new_cache;

    /* Compute any missing terms using the recurrence. */
    idx_t start = cache_size;
    if (start < 3) start = 3;          /* the first three are already known */
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = (seq_t)13 * cache[i - 1] - (seq_t)36 * cache[i - 2];
    }

    cache_size = n + 1;
    return cache[n];
}
