/*
 * A390190  a(n) is the number of square tiles in ring n (n >= 0) symmetric
 *          about a vertex of the hyperbolic regular tiling with Schläfli
 *          symbol {4,5}.
 * https://oeis.org/A390190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2) with a(0)=5, a(1)=25
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the two base cases directly. */
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)25;

    /* Persistent cache for already computed terms. */
    static seq_t *cache = NULL;
    static idx_t last_index = 1;   /* we already know a[0] and a[1] */

    /* First time: allocate a small cache that can hold at least indices 0 and 1. */
    if (cache == NULL) {
        size_t init_size = (n >= 1 ? 2 : n + 1);   /* at least two entries */
        cache = (seq_t*)malloc(sizeof(seq_t) * init_size);
        if (cache == NULL) return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)5;
        cache[1] = (seq_t)25;
        last_index = 1;
    }

    /* If the requested index is beyond what we have, grow the cache and
       fill in the missing terms using the recurrence. */
    if (n > last_index) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
        if (tmp == NULL) return (seq_t)0;   /* allocation failed */
        cache = tmp;

        for (idx_t i = last_index + 1; i <= n; ++i)
            cache[i] = (seq_t)4 * cache[i - 1] - cache[i - 2];

        last_index = n;
    }

    return cache[n];
}
