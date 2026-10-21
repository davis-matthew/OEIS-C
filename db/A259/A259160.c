/*
 * A259160  Positive squares (A000290) that are octagonal numbers (A000567)
 *          divided by 2.
 * https://oeis.org/A259160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = y(n)^2, where y(1) = 2, y(2) = 198, and y(n) = 98*y(n-1)
 *            – y(n-2) for n > 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1; if n < 1 we return 0 (unused). */
    if (n < 1) return 0;

    /* Static cache of the y sequence. */
    static seq_t *y = NULL;     /* cache array (indices 0..cap-1) */
    static size_t y_cap = 0;    /* current capacity of the cache */
    static idx_t y_max = 0;     /* highest index already computed */

    /* Ensure the cache can hold indices up to n. */
    ensure_cache(n + 1, &y, &y_cap);

    /* Initialize base cases if not yet done. */
    if (y_max < 1) {
        y[1] = (seq_t)2;
        y_max = 1;
    }
    if (y_max < 2 && n >= 2) {
        y[2] = (seq_t)198;
        y_max = 2;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = y_max + 1; i <= n; ++i) {
        /* y(i) = 98*y(i-1) – y(i-2) */
        y[i] = y[i - 1] * (seq_t)98 - y[i - 2];
    }
    y_max = n;

    /* a(n) = y(n)^2 */
    return y[n] * y[n];
}
