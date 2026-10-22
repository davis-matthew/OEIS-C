/*
 * A361649  a(n) = (1+n)*(2*a(n-1) - (n-2)*a(n-2)) with a(0) = a(1) = 1.
 * https://oeis.org/A361649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*(2*a(n-1) - (n-2)*a(n-2)), a(0) = a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of stored values */

    if (n < 2)
        return (seq_t)1;

    if (cache_len <= n) {
        idx_t new_len = n + 1;
        /* reallocate if necessary */
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

        for (idx_t i = cache_len; i <= n; ++i) {
            if (i <= 1) {
                cache[i] = (seq_t)1;
            } else {
                seq_t term1 = (seq_t)2 * cache[i - 1];
                seq_t term2 = (seq_t)(i - 2) * cache[i - 2];
                cache[i] = (seq_t)(i + 1) * (term1 - term2);
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
