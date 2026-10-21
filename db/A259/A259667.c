/*
 * A259667  Catalan numbers mod 6.
 * https://oeis.org/A259667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan number C_n modulo 6 (C_0 = 1, C_{n+1} = Σ_{i=0}^n
 *            C_i * C_{n-i} for n ≥ 0)
 * Range:     u8 n <= 119, u16 n <= 119, u32 n <= 119, u64 n <= 119,
 *            u128 n <= 119, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of Catalan numbers modulo 6 */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of values currently stored (i.e. cache_len == k means cache[0..k-1] filled) */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* cast index to a size_t (safe for the expected input range) */
    size_t k = (size_t)n;

    /* initialize cache to contain C_0 = 1 */
    if (cache_len == 0) {
        cache_cap = 16;            /* initial capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) {              /* memory allocation failed, abort */
            abort();
        }
        cache[0] = 1;              /* C_0 mod 6 */
        cache_len = 1;
    }

    /* ensure capacity for up to index k */
    if (k + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (k + 1 > new_cap)
            new_cap = new_cap * 2 + 1;   /* grow until enough */
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {               /* allocation failed */
            abort();
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing values up to k */
    for (size_t j = cache_len; j <= k; ++j) {
        /* compute C_j modulo 6 */
        unsigned long long sum = 0;
        for (size_t i = 0; i < j; ++i) {
            sum += (unsigned long long)cache[i] * (unsigned long long)cache[j - 1 - i];
        }
        cache[j] = (seq_t)(sum % 6);
    }

    cache_len = k + 1;
    return cache[k];
}
