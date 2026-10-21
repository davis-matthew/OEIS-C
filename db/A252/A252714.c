/*
 * A252714  Number of (n+2) X (3+2) 0..3 arrays with every consecutive three
 *          elements in every row and diagonal having exactly two distinct
 *          values, and in every column and antidiagonal not having exactly
 *          two distinct values, and new values 0 upwards introduced in row
 *          major order.
 * https://oeis.org/A252714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-3) - 3*a(n-4) for n>6, with a(1)=70,
 *            a(2)=50, a(3)=62, a(4)=80, a(5)=119, a(6)=170
 * Range:     u8 n <= 7, u16 n <= 22, u32 n <= 52, u64 n <= 113, u128 n <= 210,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* n<1 are undefined in OEIS but return 0 */

    /* Static memoisation table */
    static seq_t *cache = NULL;
    static size_t cap = 0;         /* allocated size (capacity)   */
    static idx_t last = 0;         /* highest index that has been computed */

    /* Initialise cache for the first six terms on first call */
    if (!cache) {
        /* Ensure space for indices 0..6 */
        ensure_cache((idx_t)6, &cache, &cap);

        cache[0] = 0;
        static const seq_t init_vals[6] = {70, 50, 62, 80, 119, 170};
        for (idx_t i = 1; i <= 6; ++i)
            cache[i] = init_vals[i - 1];

        last = 6;
    }

    /* If the requested n is already computed, return it */
    if (n <= last)
        return cache[n];

    /* Grow the cache if necessary */
    if (n > (idx_t)(cap - 1))
        ensure_cache(n, &cache, &cap);

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i)
        cache[i] = cache[i - 1] + (cache[i - 3] * (seq_t)3) - (cache[i - 4] * (seq_t)3);

    last = n;
    return cache[n];
}
