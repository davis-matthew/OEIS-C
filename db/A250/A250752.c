/*
 * A250752  Number of (n+1) X (5+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)+x(i-1,j) in the j direction.
 * https://oeis.org/A250752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3); initial values: a(1) =
 *            294, a(2) = 957, a(3) = 3066
 * Range:     u8 none, u16 n <= 5, u32 n <= 15, u64 n <= 35, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)                /* offset is 1, undefined for n = 0 */
        return 0;

    static seq_t *cache   = NULL;   /* stage‑wise cache, 1‑based index */
    static size_t cache_cap = 0;    /* allocated capacity (elements) */
    static idx_t  cache_max = 0;    /* highest n that has been computed */

    /* Grow the array if necessary.  A guard zone avoids many reallocs. */
    size_t needed = (size_t)n + 4;
    if (needed > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp)                /* allocation failed – give up */
            return 0;
        cache     = tmp;
        cache_cap = needed;
    }

    /* Initialise the first three terms once. */
    if (cache_max < 3) {
        cache[1] = (seq_t)294;
        cache[2] = (seq_t)957;
        cache[3] = (seq_t)3066;
        cache_max = 3;
    }

    /* If the requested value is already cached, return it. */
    if (n <= cache_max)
        return cache[n];

    /* Compute values successively up to n using the recurrence. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        cache[i] = cache[i-1]*6u - cache[i-2]*11u + cache[i-3]*6u;
    }

    cache_max = n;
    return cache[n];
}
