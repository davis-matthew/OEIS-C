/*
 * A270048  a(1) = 0; a(n+1) = a(n) + n * the number of digits of a(n).
 * https://oeis.org/A270048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 0; a(n) = a(n-1) + (n-1) * digit_len(a(n-1)) (n ≥ 2)
 * Range:     u8 n <= 15, u16 n <= 166, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* Cache computed terms: cache[1] = a(1), … */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;    /* largest index stored in cache */

    /* initialize cache with only the first term if it doesn't exist yet */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));   /* indices 0..1 */
        if (!cache) return (seq_t)0;                   /* out‑of‑memory guard */
        cache[1] = (seq_t)0;                           /* a(1) = 0   */
        cache_len = 1;
    }

    /* extend the cache up to the requested index n */
    if (n > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                     /* out‑of‑memory guard */
        cache = tmp;

        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            int d = digit_len(prev);                    /* #digits of a(i‑1)   */
            seq_t add = (seq_t)(i - 1) * (seq_t)d;      /* (i‑1) * digit_len */
            cache[i] = prev + add;                      /* recurrence          */
        }
        cache_len = n;
    }
    return cache[n];
}
