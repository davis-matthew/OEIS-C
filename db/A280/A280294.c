/*
 * A280294  a(n) = a(n-1) + 2^n * a(n-2) with a(0) = 1 and a(1) = 1.
 * https://oeis.org/A280294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 14, u128 n <= 21,
 *            big any n
 * Helpers:   pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 2^n * a(n-2), with a(0)=1, a(1)=1 */

#include <stdlib.h>
#include "../utils/pow2.h"   /* pow2(idx_t exp) -> seq_t */
#include "../utils/seq_mul.h" /* seq_mul(seq_t a, seq_t b) -> seq_t */

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;   /* array of seq_t */
    static size_t cache_cap = 0;  /* allocated capacity */
    static idx_t cache_last = 1;  /* highest n already computed */

    /* base cases a(0)=1, a(1)=1 */
    if (n < 2)
        return (seq_t)1;

    /* initialize cache on first call */
    if (!cache) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
    }

    /* extend the cache up to n if necessary */
    if (n > cache_last) {
        size_t need = n + 1;
        if (need > cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < need) new_cap = need;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        for (idx_t i = cache_last + 1; i <= n; ++i) {
            /* term = a(i-1) + 2^i * a(i-2) */
            seq_t prod = seq_mul(pow2((idx_t)i), cache[i - 2]);
            cache[i] = cache[i - 1] + prod;
        }
        cache_last = n;
    }

    return cache[n];
}
