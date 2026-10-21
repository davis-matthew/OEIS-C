/*
 * A298271  Expansion of x/((1 - x)*(1 - 322*x + x^2)).
 * https://oeis.org/A298271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 323*a(n-1) - 323*a(n-2) + a(n-3) (n ≥ 3) a(0) = 0, a(1) =
 *            1, a(2) = 323
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n >= cache_len) {
        /* enlarge the cache to size n+1 */
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* allocation failure – not expected for n ≤ 10000 */
            return (seq_t)0;
        }
        cache = tmp;

        /* compute terms from old length up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)0;
            } else if (i == 1) {
                cache[i] = (seq_t)1;
            } else if (i == 2) {
                cache[i] = (seq_t)323;
            } else {
                cache[i] = (seq_t)((seq_t)323 * cache[i - 1]
                                   - (seq_t)323 * cache[i - 2]
                                   + cache[i - 3]);
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
