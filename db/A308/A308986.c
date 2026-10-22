/*
 * A308986  Expansion of Product_{k>=0} 1/(1 + 2*x^(2^k)).
 * https://oeis.org/A308986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = -∑_{k=1}^n 2^popcount(k) * a(n-k)
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   popcount_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/popcount_u64.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_cnt = 0;

static void grow_cache(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < min_cap)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Ensure we can hold a[n] */
    grow_cache((size_t)n + 1);

    /* Initialise a(0) if not yet done */
    if (cache_cnt == 0) {
        cache[0] = (seq_t)1;
        cache_cnt = 1;
    }

    /* Compute any missing terms up to n */
    if (cache_cnt <= n) {
        for (idx_t i = cache_cnt; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= i; ++k) {
                int pc = popcount_u64((uint64_t)k);
                seq_t coeff = ((seq_t)1) << pc;          /* 2^popcount(k) */
                sum -= coeff * cache[(size_t)(i - k)];
            }
            cache[(size_t)i] = sum;
        }
        cache_cnt = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
