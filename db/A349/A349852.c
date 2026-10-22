/*
 * A349852  Expansion of Sum_{k>=0} k * x^k/(1 + k * x).
 * https://oeis.org/A349852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ∑_{k=0..n} (-1)^{n-k} k^(n-k+1)
 * Range:     i8 n <= 9, i16 n <= 13, i32 n <= 21, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow_seq.h"

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Grow the cache to contain a(0)...a(n) */
    if (cache == NULL || cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) exit(1);                     /* memory failure */
        cache = new_cache;
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 0; k <= i; ++k) {
                seq_t term = pow_seq((seq_t)k, (int)(i - k + 1));
                if ((i - k) & 1)                  /* (-1)^(i-k) */
                    term = -term;
                sum += term;
            }
            cache[i] = sum;
        }
        cache_size = new_size;
    }
    return cache[n];
}
