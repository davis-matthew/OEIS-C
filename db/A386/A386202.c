/*
 * A386202  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/3)} binomial(n-1,3*k) *
 *          a(3*k) * a(n-1-3*k).
 * https://oeis.org/A386202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/3)} binomial(n-1,3*k) *
 *            a(3*k) * a(n-1-3*k)
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* Grow the cache to at least n+1 entries. */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* allocation failure */
        cache = tmp;
        /* Compute missing entries. */
        for (idx_t i = (idx_t)cache_cap; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;    /* a(0) */
            } else {
                seq_t sum = (seq_t)0;
                idx_t maxk = (i - 1) / 3;
                for (idx_t k = 0; k <= maxk; ++k) {
                    seq_t bin = binomial(i - 1, 3 * k);
                    seq_t prod = bin * cache[3 * k] * cache[i - 1 - 3 * k];
                    sum += prod;
                }
                cache[i] = sum;
            }
        }
        cache_cap = new_cap;
    }
    return cache[n];
}
