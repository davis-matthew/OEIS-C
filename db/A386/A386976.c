/*
 * A386976  a(n) = Sum_{k=0..n} binomial(3*n+2,4*k).
 * https://oeis.org/A386976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(3*n+2,4*k) Recurrence: a(n) =
 *            4*a(n-1) + 24*a(n-2) + 64*a(n-3), n > 2 Base values: a(0)=1,
 *            a(1)=6, a(2)=72
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

static void ensure_cache(idx_t n) {
    if (n < cache_len) return;
    size_t new_len = (size_t)(n + 1);
    if (cache_len == 0) {
#ifdef __cplusplus
        cache = new seq_t[new_len];
#else
        cache = (seq_t*)malloc(new_len * sizeof(seq_t));
#endif
    } else {
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_len];
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
        cache = new_cache;
#else
        cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
#endif
    }
    size_t old_len = cache_len;
    cache_len = new_len;
    for (size_t i = old_len; i < new_len; ++i) {
        if (i == 0)          cache[i] = (seq_t)1;
        else if (i == 1)     cache[i] = (seq_t)6;
        else if (i == 2)     cache[i] = (seq_t)72;
        else                 cache[i] = 4 * cache[i - 1] + 24 * cache[i - 2] + 64 * cache[i - 3];
    }
}

seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[n];
}
