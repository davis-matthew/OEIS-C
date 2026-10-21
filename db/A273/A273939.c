/*
 * A273939  a(0) = 1, a(1) = 2; for k>0, a(2*k) = k*a(2*k-1) + a(2*k-2),
 *          a(2*k+1) = a(2*k) + a(2*k-1).
 * https://oeis.org/A273939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1,a(1)=2; for k>0: a(2k)=k*a(2k-1)+a(2k-2),
 *            a(2k+1)=a(2k)+a(2k-1)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 37, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values (O(1) look‑up) */
    static seq_t *cache    = NULL;
    static size_t cache_cap = 0;
    static size_t computed  = 0;  /* number of terms already computed (next index to compute) */

    /* enlarge cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);           /* memory allocation failed */
        cache    = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first two terms lazily */
    if (computed == 0) {
        cache[0] = (seq_t)1;
        computed = 1;
    }
    if (computed == 1 && n >= 1) {
        cache[1] = (seq_t)2;
        computed = 2;
    }

    /* compute missing terms up to n */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        if ((i & 1) == 0) {                     /* i = 2*k  */
            idx_t k = i >> 1;
            cache[i] = (seq_t)k * cache[i-1] + cache[i-2];
        } else {                                 /* i = 2*k+1  */
            cache[i] = cache[i-1] + cache[i-2];
        }
    }
    computed = (size_t)n + 1;
    return cache[n];
}
