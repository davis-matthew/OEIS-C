/*
 * A253209  a(n) = 6^n + 5.
 * https://oeis.org/A253209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n + 5
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static size_t cache_cap = 0;

static void grow_cache(size_t new_cap) {
#ifdef __cplusplus
    seq_t *newp = new seq_t[new_cap];
    for (idx_t i = 0; i < cache_size; ++i) newp[i] = cache[i];
    delete [] cache;
    cache = newp;
#else
    seq_t *newp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = newp;
#endif
    cache_cap = new_cap;
}

static void init_cache(void) {
    if (cache_cap < 2) cache_cap = 2;
#ifdef __cplusplus
    cache = new seq_t[cache_cap];
#else
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
#endif
    cache[0] = (seq_t)6;
    cache[1] = (seq_t)11;
    cache_size = 2;
}

seq_t a(idx_t n) {
    if (cache_size == 0) {
        init_cache();
        if (n == 0) return cache[0];
        if (n == 1) return cache[1];
    } else if (n < (idx_t)cache_size) {
        return cache[n];
    }

    if (n >= (idx_t)cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 2;
        while (new_cap <= (size_t)n) new_cap *= 2;
        grow_cache(new_cap);
    }

    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = (seq_t)7 * cache[i-1] - (seq_t)6 * cache[i-2];
    }
    cache_size = n + 1;
    return cache[n];
}
