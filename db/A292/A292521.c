/*
 * A292521  a(n) = a(n-2) - 2a(n-3) + a(n-4) for n>3, with a(0)=2, a(1)=0,
 *          a(2)=1, a(3)=-1, a sequence related to Pellian numbers.
 * https://oeis.org/A292521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-2) - 2*a(n-3) + a(n-4) for n>3; a(0)=2, a(1)=0,
 *            a(2)=1, a(3)=-1
 * Range:     i8 n <= 12, i16 n <= 23, i32 n <= 39, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void init_cache(void) {
    if (cache) return;
    cache_len = 4;
    cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
    cache[0] = (seq_t)2;
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)1;
    cache[3] = (seq_t)-1;
}

static void extend_cache(idx_t n) {
    if (!cache) init_cache();
    if (n < cache_len) return;
    idx_t old_len = cache_len;
    cache_len = n + 1;
    cache = (seq_t*)realloc(cache, cache_len * sizeof(seq_t));
    for (idx_t i = old_len; i < cache_len; ++i) {
        cache[i] = cache[i-2] - (seq_t)2 * cache[i-3] + cache[i-4];
    }
}

seq_t a(idx_t n) {
    extend_cache(n);
    return cache[n];
}
