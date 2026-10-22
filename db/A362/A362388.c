/*
 * A362388  a(n) = a(n-1) + a(n-3) + a(n-4), where a(0) = 1, a(1) = 2, a(2) =
 *          5, a(3) = 7.
 * https://oeis.org/A362388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4), a(0) = 1, a(1) = 2, a(2) = 5,
 *            a(3) = 7
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for already computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static int initialized = 0;

    /* Ensure the cache can hold indices up to n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 8) new_cap = 8;          /* small default size */
        if (!cache) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        if (!cache) return (seq_t)0;           /* allocation failure */
        cap = new_cap;
    }

    /* Initialise the first four terms on the first run */
    if (!initialized) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)7;
        initialized = 1;
    }

    /* Compute missing terms up to n */
    for (size_t i = 4; i <= (size_t)n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 3] + cache[i - 4];
    }

    return cache[(size_t)n];
}
