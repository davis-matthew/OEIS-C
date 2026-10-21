/*
 * A289557  Expansion of Hypergeometric function F(1/12, 7/12; 1; 1728*x) in
 *          powers of x.
 * https://oeis.org/A289557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=a(n-1)*12*(12*n-5)*(12*n-11)/(n*n) for n>=1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;

/* ensure_cache makes sure that cache[0..n] are computed */
static void ensure_cache(idx_t n) {
    if (cache_len > n) return;

    /* resize if needed */
    size_t need = (size_t)n + 1;
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                                 /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }
        seq_t prev = cache[i-1];

        seq_t val = prev;
        val *= (seq_t)12;

        idx_t t1 = (idx_t)12 * i - 5;
        idx_t t2 = (idx_t)12 * i - 11;
        val *= (seq_t)t1;
        val *= (seq_t)t2;

        seq_t denom = (seq_t)i * (seq_t)i;
        cache[i] = val / denom;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[n];
}
