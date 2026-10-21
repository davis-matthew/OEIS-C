/*
 * A295276  a(n) = number of earlier terms (with multiplicity) that have no
 *          common one bit with n in binary representation.
 * https://oeis.org/A295276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{ k | 0 < k < n and a(k) AND n = 0 } (OEIS A295276)
 * Range:     u8 n <= 319, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* dynamic array: cache[i-1] == a(i) */
static idx_t cache_len = 0;      /* number of values stored in cache (max n computed) */

seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for the series, return 0 */
        return 0;

    /* Extend the cache if needed */
    if (cache_len < n) {
        idx_t new_len = n;
        size_t new_size = sizeof(seq_t) * new_len;
        seq_t *tmp = NULL;
        if (cache == NULL)
            tmp = (seq_t *)malloc(new_size);
        else
            tmp = (seq_t *)realloc(cache, new_size);
        if (tmp == NULL)      /* allocation failed – abort */
            return 0;
        cache = tmp;
        /* Compute missing values */
        for (idx_t i = cache_len + 1; i <= new_len; ++i) {
            seq_t cnt = 0;
            /* Count earlier terms having no common 1-bits with i */
            for (idx_t j = 0; j < i - 1; ++j) {
                if ((cache[j] & (seq_t)i) == 0)
                    ++cnt;
            }
            cache[i - 1] = cnt;
        }
        cache_len = new_len;
    }
    return cache[n - 1];
}
