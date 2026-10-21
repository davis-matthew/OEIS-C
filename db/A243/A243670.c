/*
 * A243670  Number of hypo-Sylvester classes of 5-packed words of degree n.
 * https://oeis.org/A243670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1)=1, a(2)=7, a(n)=8*a(n-1)-2*a(n-2) for n≥3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (n == 0) return 0;             /* Undefined for index 0 */

    /* Expand cache if needed */
    if ((size_t)n > cache_size) {
        size_t new_size = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) { /* Allocation failure – recompute without cache */
            seq_t prev2 = 1;   /* a(1) */
            if (n == 1) return 1;
            seq_t prev1 = 7;   /* a(2) */
            if (n == 2) return 7;
            seq_t cur;
            for (size_t i = 3; i <= new_size; ++i) {
                cur = 8 * prev1 - 2 * prev2;
                prev2 = prev1;
                prev1 = cur;
            }
            return cur;
        }
        cache = tmp;
        size_t old_size = cache_size;

        /* Initialize base terms if they are newly added */
        if (old_size == 0 && new_size >= 1) cache[0] = 1;       /* a(1) */
        if (old_size <= 1 && new_size >= 2) cache[1] = 7;       /* a(2) */

        /* Compute new terms using the recurrence */
        size_t start = (old_size > 2 ? old_size : 2);
        for (size_t i = start; i < new_size; ++i) {
            cache[i] = 8 * cache[i - 1] - 2 * cache[i - 2];
        }
        cache_size = new_size;
    }

    return cache[(size_t)n - 1];
}
