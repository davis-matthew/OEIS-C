/*
 * A269104  Number of n X 3 0..3 arrays with some element plus some
 *          horizontally or vertically adjacent neighbor totalling three no
 *          more than once.
 * https://oeis.org/A269104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 36*a(n-1) -
 *            378*a(n-2) + 972*a(n-3) - 729*a(n-4) for n > 5 * with initial
 *            values * a(1) = 60, a(2) = 1740, a(3) = 40908, * a(4) = 872460,
 *            a(5) = 17593092. * The function memoises already-computed terms
 *            so that * calls with arbitrary order of n remain...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation.  Index 0 is unused. */
    static seq_t *cache = NULL;
    static size_t max_computed = 0;          /* highest index already computed */

    /* Ensure the cache is large enough for index n. */
    size_t needed = (size_t)n + 1;           /* include index 0 */
    if (cache == NULL) {
        cache = (seq_t*)realloc(NULL, needed * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        for (size_t i = 0; i < needed; ++i) cache[i] = 0;
        max_computed = 0;
    } else if (needed > max_computed + 1) {
        cache = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        for (size_t i = max_computed + 1; i < needed; ++i) cache[i] = 0;
        max_computed = 0;  /* will be recomputed below */
    }

    /* Initialise the known initial values. */
    if (n <= 5) {
        static const seq_t init[6] = {0, 60, 1740, 40908, 872460, 17593092};
        return init[(size_t)n];
    }

    /* Compute any missing terms up to n. */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        if (i <= 5) {
            static const seq_t init[6] = {0, 60, 1740, 40908, 872460, 17593092};
            cache[i] = init[i];
            continue;
        }
        seq_t t1 = (seq_t)36 * cache[i-1] + (seq_t)972 * cache[i-3];
        seq_t t2 = (seq_t)378 * cache[i-2] + (seq_t)729 * cache[i-4];
        cache[i] = t1 - t2;                     /* t1 > t2 for i > 5 */
    }
    max_computed = (size_t)n;
    return cache[(size_t)n];
}
