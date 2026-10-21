/*
 * A276927  Number of ordered set partitions of [n] with at most seven
 *          elements per block.
 * https://oeis.org/A276927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{min(7,n)} binom(n,k)*a(n-k), a(0)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

static const seq_t fact[8] = {
    (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)6,
    (seq_t)24, (seq_t)120, (seq_t)720, (seq_t)5040
};

static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;

    if (!cache) {
        cache_cap = n + 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    } else if (n >= cache_cap) {
        cache_cap = n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
    }

    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t maxk = (i < 7) ? i : 7;
        for (idx_t k = 1; k <= maxk; ++k) {
            seq_t binom = (seq_t)1;
            for (idx_t t = 1; t <= k; ++t) {
                binom *= (seq_t)(i - t + 1);
            }
            binom /= fact[k];
            sum += binom * cache[i - k];
        }
        cache[i] = sum;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
