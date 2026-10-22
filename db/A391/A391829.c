/*
 * A391829  a(n) = (1/4) * Sum_{k=0..n} (k+2) * binomial(2*k+2,2*n-2*k+1).
 * https://oeis.org/A391829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 11*a(n-4) - 2*a(n-6) + 4*a(n-7) -
 *            a(n-8) initial values: a(0)=1, a(1)=3, a(2)=9, a(3)=30, a(4)=91,
 *            a(5)=271, a(6)=801, a(7)=2330
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

static void ensure_cache_cap(idx_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap <= (size_t)needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        abort();
    }
    cache = tmp;
    cache_cap = (idx_t)new_cap;
}

static void init_cache(void)
{
    if (cache_len > 0)
        return;
    ensure_cache_cap(8);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)3;
    cache[2] = (seq_t)9;
    cache[3] = (seq_t)30;
    cache[4] = (seq_t)91;
    cache[5] = (seq_t)271;
    cache[6] = (seq_t)801;
    cache[7] = (seq_t)2330;
    cache_len = 8;
}

seq_t a(idx_t n)
{
    init_cache();
    if (n < cache_len)
        return cache[n];
    for (idx_t i = cache_len; i <= n; ++i) {
        ensure_cache_cap(i + 1);
        seq_t val = (seq_t)4 * cache[i - 1];
        val -= (seq_t)2 * cache[i - 2];
        val -= (seq_t)11 * cache[i - 4];
        val -= (seq_t)2 * cache[i - 6];
        val += (seq_t)4 * cache[i - 7];
        val -= cache[i - 8];
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
