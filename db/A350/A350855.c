/*
 * A350855  a(0) = 1, a(n) = (n+1)*a(n-1) + (n-2).
 * https://oeis.org/A350855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(n) = (n+1)*a(n-1)+(n-2) for n≥1 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;

    /* Ensure the cache holds values up to n */
    if (n + 1 > cache_sz) {
        size_t new_sz = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) abort();          /* memory allocation failed */
        cache = tmp;

        if (cache_sz == 0) {
            cache[0] = 1;           /* a(0) */
            cache_sz = 1;
        }

        /* Compute missing terms */
        idx_t start = cache_sz;
        if (start < 1) start = 1;
        for (idx_t i = start; i <= n; ++i) {
            seq_t val = ((seq_t)(i + 1)) * cache[i - 1];
            if (i == 1) {
                val -= 1;           /* a(1) = (1+1)*a(0) + (1-2) = 2*1 -1 */
            } else if (i >= 2) {
                val += (seq_t)(i - 2);
            }
            cache[i] = val;
        }
        cache_sz = n + 1;
    }
    return cache[n];
}
