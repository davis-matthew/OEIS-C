/*
 * A321703  a(n) = 3*a(n-1) + 4*a(n-2) + a(n-3), a(0) = 1, a(1) = 1, a(2) = 5.
 * https://oeis.org/A321703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) + a(n-3), a(0)=1, a(1)=1, a(2)=5
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base cases
       These return immediately without allocating memory. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;

    /* Cache needed only for n >= 3.  Static members keep state across calls. */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;          /* number of elements already computed   */
    static size_t cache_cap = 0;          /* allocated capacity (in seq_t elements) */

    /* Allocate (or enlarge) the cache to hold up to n. */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if ((size_t)(n + 1) > cache_cap) {
        cache_cap = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* If nothing is cached yet, initialize the seed values. */
    if (cached_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cached_len = 3;
    }

    /* Return the value if it has already been computed. */
    if (cached_len > n)
        return cache[n];

    /* Compute any missing terms up to n using the recurrence. */
    for (idx_t i = cached_len; i <= n; ++i) {
        cache[i] = (seq_t)(3 * cache[i-1] + 4 * cache[i-2] + cache[i-3]);
    }
    cached_len = n + 1;
    return cache[n];
}
