/*
 * A297443  a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3) + 3*a(n-4) - 3*a(n-5), where
 *          a(0) = 1, a(1) = 3, a(2) = 6, a(3) = 11, a(4) = 20, a(5) = 33.
 * https://oeis.org/A297443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+2*a(n-2)-2*a(n-3)+3*a(n-4)-3*a(n-5) initial
 *            values: a(0)=1,a(1)=3,a(2)=6,a(3)=11,a(4)=20,a(5)=33
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 38, u64 n <= 79, u128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Initial terms, stored as constant array for both C and C++ */
static const seq_t init_vals[6] = { (seq_t)1, (seq_t)3, (seq_t)6,
                                    (seq_t)11, (seq_t)20, (seq_t)33 };

/* Dynamic cache for memoised values; works for C and C++ */
static seq_t *cache = NULL;      /* array holding computed values   */
static size_t cache_cap = 0;      /* current allocated capacity      */
static idx_t   max_computed = (idx_t)-1;   /* highest index computed        */

/* Grow the cache so that index n is reachable */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n < cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 6;
    if (new_cap <= (size_t)n) {
        while (new_cap <= (size_t)n) new_cap <<= 1;
    }

#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
#else
    seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
    if (!tmp) abort();

    /* Copy old data */
    if (cache) {
#ifdef __cplusplus
        for (size_t i = 0; i < cache_cap; ++i) tmp[i] = cache[i];
        delete[] cache;
#else
        memcpy(tmp, cache, cache_cap * sizeof(seq_t));
        free(cache);
#endif
    }

    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise cache with the first six values */
static void init_cache(void)
{
    if (cache) return;
    ensure_cache_capacity(5);
    for (size_t i = 0; i < 6; ++i) cache[i] = init_vals[i];
    max_computed = 5;
}

/* Return a(n) for any n >= 0, with memoisation and arbitrary order */
seq_t a(idx_t n)
{
    if (n <= 5) return init_vals[n];

    init_cache();

    if (n > max_computed) {
        ensure_cache_capacity(n);
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            cache[i] = cache[i-1]
                     + (seq_t)2 * cache[i-2]
                     - (seq_t)2 * cache[i-3]
                     + (seq_t)3 * cache[i-4]
                     - (seq_t)3 * cache[i-5];
        }
        max_computed = n;
    }
    return cache[n];
}
