/*
 * A301674  Coordination sequence for node of type V1 in "krs" 2-D tiling (or
 *          net).
 * https://oeis.org/A301674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p(n) - a(n-1) + 2*a(n-3) + 2*a(n-4) - a(n-6) - a(n-7);
 *            where p(n) = [1,5,12,20,20,18,5,1,-2] for n = 0..8, and p(n)=0
 *            for n≥9
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;      /* memoized a(n) values */
static idx_t cache_size = 0;     /* number of computed entries */
static idx_t cache_cap   = 0;     /* capacity of cache */

static void ensure_capacity(idx_t n)
{
    if (cache_cap > n) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= n) new_cap <<= 1;

    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
    for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
    delete[] cache;
#else
    new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
    for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
    free(cache);
#endif
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    const long long p[9] = {1, 5, 12, 20, 20, 18, 5, 1, -2};

    if (n >= cache_size) {
        ensure_capacity(n);
        for (idx_t i = cache_size; i <= n; ++i) {
            long long val = (i < 9 ? p[i] : 0);
            if (i >= 1) val -= (long long)cache[i-1];
            if (i >= 3) val += 2LL * (long long)cache[i-3];
            if (i >= 4) val += 2LL * (long long)cache[i-4];
            if (i >= 6) val -= (long long)cache[i-6];
            if (i >= 7) val -= (long long)cache[i-7];
            cache[i] = (seq_t)val;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
