/*
 * A326290  Number of non-crossing n-vertex graphs with loops.
 * https://oeis.org/A326290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) + (1/2)*∑_{k=2}^{n-3} a(k+1)*a(n-k) (n ≥ 4)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=1, a(1)=2, a(2)=8, a(3)=64. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache to allow arbitrary order of calls */
    static seq_t *cache = NULL;          /* array of computed a(i) */
    static size_t cache_cap = 0;         /* current capacity of cache */
    static idx_t last_computed = 0;      /* highest index already filled (>=3) */

    /* initialise cache with the first four terms */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;      /* error, but should not happen */
        cache[0] = (seq_t)1;    /* a0 */
        cache[1] = (seq_t)2;    /* a1 */
        cache[2] = (seq_t)8;    /* a2 */
        cache[3] = (seq_t)64;   /* a3 */
        last_computed = 3;
    }

    /* grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* error, but should not happen */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms that are not yet cached */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 3) continue;                 /* initial values already set */

        seq_t sum = 0;
        /* sum_{k=2}^{i-3} a(k+1) * a(i-k)   (indices are idx_t) */
        for (idx_t k = 2; k <= i - 3; ++k) {
            sum += cache[k + 1] * cache[i - k];
        }

        /* a(i) = 12*a(i-1) + sum/2  (sum is guaranteed even) */
        cache[i] = (12 * cache[i - 1]) + (sum / 2);
    }

    last_computed = n;
    return cache[(size_t)n];
}
