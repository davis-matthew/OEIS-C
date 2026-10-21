/*
 * A298419  a(n) = n*a(n-1) - a(n-2) + a([n/2]), where a(0) = 1, a(1) = 1,
 *          a(2) = 1.
 * https://oeis.org/A298419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*a(n-1) - a(n-2) + a(floor(n/2)), a(0) = 1, a(1) = 1,
 *            a(2) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    /* grow the cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = (cache_cap ? cache_cap * 2 : 1);
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure – caller can handle */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values up to n (O(n) time) */
    for (idx_t i = 0; i <= n; ++i) {
        if (i <= 2) {
            cache[i] = 1;
        } else {
            cache[i] = (seq_t)i * cache[i - 1] - cache[i - 2] + cache[i / 2];
        }
    }
    return cache[n];
}
