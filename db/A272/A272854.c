/*
 * A272854  Ramanujan's beta-series.
 * https://oeis.org/A272854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 82*a(n-1) + 82*a(n-2) - a(n-3)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

//          a(0) = 10, a(1) = 812, a(2) = 67402
#include <stdlib.h>

seq_t a(idx_t n)
{
    // Static cache to store computed values.
    static seq_t *cache = NULL;
    static size_t size_cache = 0;    // allocated size
    static idx_t last_computed = (idx_t)(-1);  // index of the largest value computed

    // Initialize cache on first call.
    if (last_computed == (idx_t)(-1))
    {
        size_cache = (size_t)3;                      // we need indices 0..2
        cache = (seq_t *)realloc(NULL, size_cache * sizeof(seq_t));
        cache[0] = (seq_t)10;
        cache[1] = (seq_t)812;
        cache[2] = (seq_t)67402;
        last_computed = 2;
    }

    // If n is already computed, return it.
    if (n <= last_computed)
        return cache[n];

    // Ensure the cache can hold up to index n.
    size_t new_size = (size_t)(n + 1);
    if (new_size > size_cache)
    {
        size_cache = new_size;
        cache = (seq_t *)realloc(cache, size_cache * sizeof(seq_t));
    }

    // Compute new terms using the recurrence.
    for (idx_t i = last_computed + 1; i <= n; ++i)
    {
        cache[i] = (seq_t)82 * cache[i - 1] + (seq_t)82 * cache[i - 2] - cache[i - 3];
    }
    last_computed = n;

    return cache[n];
}
