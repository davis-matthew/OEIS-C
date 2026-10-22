/*
 * A392270  a(n) = Sum_{k=0..floor(2*n/5)} binomial(k+2,2) *
 *          binomial(k,2*n-5*k).
 * https://oeis.org/A392270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(2*n/5)} binomial(k+2,2) *
 *            binomial(k,2*n-5*k)
 * Range:     u8 n <= 15, u16 n <= 32, u32 n <= 70, u64 n <= 152,
 *            u128 n <= 322, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;            /* number of cached entries (size) */

    /* If the cache does not exist yet, allocate and compute up to n */
    if (cache == NULL) {
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failed */

        for (idx_t i = 0; i <= n; ++i) {
            seq_t sum = 0;
            idx_t kmax = (2 * i) / 5;
            for (idx_t k = 0; k <= kmax; ++k) {
                seq_t t1 = ((seq_t)(k + 2) * (seq_t)(k + 1)) / (seq_t)2;
                idx_t rhs = 2 * i - 5 * k;
                seq_t t2 = binomial(k, rhs);
                sum += t1 * t2;
            }
            cache[i] = sum;
        }
        cache_len = n + 1;
    }
    /* If a larger value is requested, extend the cache */
    else if (n >= cache_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;      /* allocation failed */
        cache = new_cache;
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = 0;
            idx_t kmax = (2 * i) / 5;
            for (idx_t k = 0; k <= kmax; ++k) {
                seq_t t1 = ((seq_t)(k + 2) * (seq_t)(k + 1)) / (seq_t)2;
                idx_t rhs = 2 * i - 5 * k;
                seq_t t2 = binomial(k, rhs);
                sum += t1 * t2;
            }
            cache[i] = sum;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
