/*
 * A356344  a(n) = Sum_{k=1..n} binomial(2*k, k) * sigma(k).
 * https://oeis.org/A356344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(2*k, k) * sigma(k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial, seq_mul, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/sigma_of.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* We keep a running prefix sum cache so that each call
       runs in O(k) time where k = max(1, n - previous_max_cached).
       The cache holds a[0] = 0, a[1], a[2], …, a[max_cached]. */

    static seq_t *cache = NULL;          /* cache[0] = 0, cache[i] = a(i) */
    static size_t cache_len = 0;         /* number of valid entries (cache_len-1 == max_cached) */

    /* Initialise cache with a[0] = 0 */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) {            /* allocation failed – abort */
            return (seq_t)0;
        }
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* If requested n is not yet cached, extend and compute new sums */
    if ((idx_t)cache_len <= n) {
        /* New length = n+1 (indices 0..n inclusive) */
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) {              /* allocation failed – abort */
            return (seq_t)0;
        }
        cache = tmp;
        /* Compute sum for indices cache_len .. new_len-1 */
        for (size_t i = (size_t)cache_len; i < new_len; ++i) {
            seq_t bin = binomial((idx_t)(2 * i), (idx_t)i);
            seq_t sigma = sigma_of((idx_t)i);
            seq_t term = seq_mul(bin, sigma);
            cache[i] = cache[i - 1] + term;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
