/*
 * A297459  Number of 3 X n 0..1 arrays with every 1 horizontally, diagonally
 *          or antidiagonally adjacent to 0 or 2 neighboring 1s.
 * https://oeis.org/A297459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) - 7*a(n-3) + 8*a(n-4) - 19*a(n-5) +
 *            8*a(n-6) + 2*a(n-8) a(1) = 8, a(2) = 19, a(3) = 92, a(4) = 285,
 *            a(5) = 1143, a(6) = 3933, a(7) = 14817, a(8) = 52868
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t _init_terms[8] = {
    (seq_t)8,
    (seq_t)19,
    (seq_t)92,
    (seq_t)285,
    (seq_t)1143,
    (seq_t)3933,
    (seq_t)14817,
    (seq_t)52868
};

static seq_t *cache = NULL;    /* 1‑based cache */
static size_t cache_cap = 0;   /* allocated size (including dummy index 0) */
static idx_t  cache_max = 0;   /* highest index computed */

/* Ensure the cache can hold indices up to n (inclusive). */
static void _ensure_cache(idx_t n)
{
    if (!cache) {
        cache_cap = 16;
        if (cache_cap <= (size_t)n) cache_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* store initial values */
        for (idx_t i = 1; i <= 8; ++i) cache[i] = _init_terms[i-1];
        cache_max = 8;
        return;
    }
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp) {
            cache = tmp;
            cache_cap = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* defined only for n>=1; safety */
    if (n <= 8) return _init_terms[n-1];

    _ensure_cache(n);

    /* compute values up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        sum += 3 * cache[i-1];
        sum += 4 * cache[i-2];
        sum += 8 * cache[i-4];
        sum += 8 * cache[i-6];
        sum += 2 * cache[i-8];
        seq_t diff = 0;
        diff += 7 * cache[i-3];
        diff += 19 * cache[i-5];
        cache[i] = sum - diff;
    }
    cache_max = n;
    return cache[n];
}
