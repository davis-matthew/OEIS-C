/*
 * A368327  a(0) = 0; a(n) = (1+a(n-1))^5 for n > 0.
 * https://oeis.org/A368327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0; a(n)=(1+a(n-1))^5 for n>0
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

static void ensure_cache(idx_t n)
{
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= n) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(*new_cache));
        if (!new_cache) abort();
        cache = new_cache;
        cache_cap = new_cap;
    }
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache_len = 1;
    }
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t inc = cache[i - 1] + (seq_t)1;
        seq_t val = inc * inc;          // (1 + a(i-1))^2
        val = val * val;                // (1 + a(i-1))^4
        val = val * inc;                // (1 + a(i-1))^5
        cache[i] = val;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
