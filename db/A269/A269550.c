/*
 * A269550  Expansion of (-5*x^2 + 228*x - 7)/(x^3 - 99*x^2 + 99*x - 1).
 * https://oeis.org/A269550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 99*a(n-1) - 99*a(n-2) + a(n-3) - c(n), where c(0) = -7,
 *            c(1) = 228, c(2) = -5, c(n)=0 for n ≥ 3
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache static storage for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* enlarge cache until it contains a(n) */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        /* realloc is returned as void* in C++: cast explicitly */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;          /* allocation failed: fallback to 0 */
        cache = tmp;

        /* compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t coeff = 0;
            if (i == 0)          coeff = (seq_t)-7;
            else if (i == 1)     coeff = (seq_t)228;
            else if (i == 2)     coeff = (seq_t)-5;

            if (i == 0)           cache[i] = (seq_t)7;
            else if (i == 1)      cache[i] = (seq_t)465;
            else if (i == 2)      cache[i] = (seq_t)45347;
            else
                cache[i] = (seq_t)99 * cache[i-1]
                         - (seq_t)99 * cache[i-2]
                         + cache[i-3]
                         - coeff;
        }
        cache_size = new_size;
    }
    return cache[n];
}
