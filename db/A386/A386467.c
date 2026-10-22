/*
 * A386467  a(0) = a(1) = 1; a(n) = a(n-1) + a(n-2) + Sum_{k=0..n-1} k * a(k)
 *          * a(n-1-k).
 * https://oeis.org/A386467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = 1; a(n) = a(n-1) + a(n-2) + Sum_{k=0..n-1}
 *            k*a(k)*a(n-1-k);
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>


seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n == 0 || n == 1) {
        return (seq_t)1;
    }

    if (cache_len <= n) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* allocation failure – return 0 for safety */
            return (seq_t)0;
        }
        cache = tmp;

        /* initialise new entries to zero */
        for (idx_t i = cache_len; i < new_len; ++i) {
            cache[i] = (seq_t)0;
        }

        /* initialise base values if this is the first allocation */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)1;
        }

        idx_t start = cache_len;
        if (start < 2) {
            start = 2;
        }

        /* compute terms from the first missing index up to n */
        for (idx_t i = start; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k < i; ++k) {
                sum += (seq_t)k * cache[k] * cache[i - 1 - k];
            }
            cache[i] = cache[i - 1] + cache[i - 2] + sum;
        }

        cache_len = new_len;
    }

    return cache[n];
}
