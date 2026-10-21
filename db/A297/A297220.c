/*
 * A297220  Number of n X 4 0..1 arrays with every 1 horizontally or
 *          antidiagonally adjacent to 1 neighboring 1s.
 * https://oeis.org/A297220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+6*a(n-2)+11*a(n-3)+6*a(n-4)+a(n-5) with a(1)=4,
 *            a(2)=16, a(3)=57, a(4)=207, a(5)=751
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* length allocated, index 0 unused */
    static idx_t max_computed = 0;

    if (n < 1)
        return (seq_t)0;  /* undefined for n < 1 */

    /* ensure the cache is large enough */
    size_t needed = (size_t)n + 1;          /* because indices start at 1 */
    if (needed > cache_len) {
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        cache_len = needed;
    }

    /* base values */
    static const seq_t base[6] = { 0, 4, 16, 57, 207, 751 };

    /* compute up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 5) {
            cache[(size_t)i] = base[(size_t)i];
        } else {
            seq_t v = cache[(size_t)(i - 1)];
            v += (seq_t)6 * cache[(size_t)(i - 2)];
            v += (seq_t)11 * cache[(size_t)(i - 3)];
            v += (seq_t)6 * cache[(size_t)(i - 4)];
            v += cache[(size_t)(i - 5)];
            cache[(size_t)i] = v;
        }
    }
    max_computed = n;
    return cache[(size_t)n];
}
