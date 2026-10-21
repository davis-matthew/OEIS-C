/*
 * A281216  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 342", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A281216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) + a(n-4) - 4*a(n-6) for n > 5 Initial values:
 *            a(0) = 1, a(1) = 3, a(2) = 4, a(3) = 14, a(4) = 17, a(5) = 59
 *            This implementation uses memoisation and linear-time recurrence
 *            evaluation
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache state */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;     /* Allocated capacity */
    static idx_t max_computed = (idx_t)-1; /* Highest n with value stored */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 6;                 /* Need at least values 0..5 */
        if (n + 1 > cache_cap) cache_cap = n + 1;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = 1;
        cache[1] = 3;
        cache[2] = 4;
        cache[3] = 14;
        cache[4] = 17;
        cache[5] = 59;
        max_computed = 5;
    } else if (n + 1 > cache_cap) { /* Grow cache if needed */
        idx_t new_cap = cache_cap;
        while (new_cap < n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) {
            /* Allocation failed: abort to avoid silent error */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return precomputed value if available */
    if (n <= max_computed) {
        return cache[n];
    }

    /* Compute missing values up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* a(i) = 4*a(i-2) + a(i-4) - 4*a(i-6) for i > 5 */
        cache[i] = (seq_t)4 * cache[i - 2] + cache[i - 4] - (seq_t)4 * cache[i - 6];
    }
    max_computed = n;
    return cache[n];
}
