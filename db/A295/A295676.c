/*
 * A295676  a(n) = a(n-1) + a(n-3) + a(n-4), where a(0) = 1, a(1) = 1, a(2) =
 *          3, a(3) = -3.
 * https://oeis.org/A295676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4); Initial values: a(0)=1, a(1)=1,
 *            a(2)=3, a(3)=-3
 * Range:     i8 n <= 16, i16 n <= 27, i32 n <= 50, i64 n <= 96, i128 n <= 188,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

static void ensure_cache(idx_t n)
{
    size_t target = (size_t)n + 1U;
    if (!cache) {
        cache = (seq_t*)malloc(target * sizeof(seq_t));
        if (!cache) return;
        cache_size = target;
        if (target > 0) cache[0] = (seq_t)1;
        if (target > 1) cache[1] = (seq_t)1;
        if (target > 2) cache[2] = (seq_t)3;
        if (target > 3) cache[3] = (seq_t)-3;
        for (idx_t i = 4; i <= n; ++i) {
            cache[i] = cache[i-1] + cache[i-3] + cache[i-4];
        }
        return;
    }
    if (target <= cache_size) return;
    seq_t *tmp = (seq_t*)realloc(cache, target * sizeof(seq_t));
    if (!tmp) return;
    cache = tmp;
    if (cache_size < 4) {
        if (target > 0) cache[0] = (seq_t)1;
        if (target > 1) cache[1] = (seq_t)1;
        if (target > 2) cache[2] = (seq_t)3;
        if (target > 3) cache[3] = (seq_t)-3;
    }
    idx_t start = cache_size < 4 ? (idx_t)4 : cache_size;
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-3] + cache[i-4];
    }
    cache_size = target;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
