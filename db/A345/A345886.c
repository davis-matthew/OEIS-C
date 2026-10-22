/*
 * A345886  a(1) = 1, a(n) = a(n-1)/3 if a(n-1) is divisible by 3, otherwise
 *          a(n) = n - a(n-1).
 * https://oeis.org/A345886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = a(n-1)/3 if a(n-1) divisible by 3, else a(n) =
 *            n - a(n-1)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* Number of allocated slots */
    static idx_t computed_up_to = 0; /* Highest n for which cache is filled */

    if (n == 0) return 0; /* Undefined, but return 0 for safety */

    /* Expand cached array if necessary */
    if ((idx_t)cache_size < n) {
        /* New size = n */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed: abort or return 0 */
            return 0;
        }
        cache = tmp;
        cache_size = n;
    }

    /* Compute missing terms up to n */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = 1;
        } else {
            seq_t prev = cache[i - 2];          /* a(i-1) */
            if (prev % 3 == 0) {
                cache[i - 1] = prev / 3;        /* a(i) = a(i-1)/3 */
            } else {
                cache[i - 1] = (seq_t)i - prev; /* a(i) = i - a(i-1) */
            }
        }
    }
    computed_up_to = n;
    return cache[n - 1];
}
