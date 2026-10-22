/*
 * A307271  Partial sums of A307201.
 * https://oeis.org/A307271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-4) - 2*a(n-5) - a(n-8) + a(n-9) for n>8;
 *            initial values: a(0)=1, a(1)=4, a(2)=13, a(3)=25, a(4)=46,
 *            a(5)=76, a(6)=106, a(7)=139, a(8)=181
 * Range:     u8 n <= 9, u16 n <= 152, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[9] = {1, 4, 13, 25, 46, 76, 106, 139, 181};
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_capacity(idx_t n)
{
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while ((idx_t)new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 9) return init[n];

    ensure_capacity(n);

    if (cache_len < 9) {
        for (idx_t i = 0; i < 9; ++i) cache[i] = init[i];
        cache_len = 9;
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = cache[i-1] + 2 * cache[i-4]
                  - 2 * cache[i-5] - cache[i-8] + cache[i-9];
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
