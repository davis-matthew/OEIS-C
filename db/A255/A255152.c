/*
 * A255152  Number of (n+2) X (1+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum 1 and no antidiagonal sum 1 and no row sum 0 or 1 and
 *          no column sum 0 or 1.
 * https://oeis.org/A255152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 3*a(n-3) + a(n-4) + 2*a(n-5) +
 *            2*a(n-6) (a(1)=28, a(2)=62, a(3)=141, a(4)=331, a(5)=773,
 *            a(6)=1782)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 23, u64 n <= 50, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n) {
    assert(n >= 1);

    /* static cache for already computed terms */
    static seq_t *cache = NULL;
    static idx_t last_computed = 0;  /* highest index for which cache is valid */
    static size_t cache_capacity = 0;/* allocated size for cache (including unused index 0) */

    /* ensure capacity for indices up to n */
    if ((size_t)(n + 1) > cache_capacity) {
        size_t new_cap = (size_t)(n + 1);
#if defined(__cplusplus)
        seq_t *tmp = reinterpret_cast<seq_t*>(realloc(cache, new_cap * sizeof(seq_t)));
#else
        seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
#endif
        if (!tmp) abort();
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* compute new values if needed */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            switch (i) {
                case 1: cache[1] = (seq_t)28;  break;
                case 2: cache[2] = (seq_t)62;  break;
                case 3: cache[3] = (seq_t)141; break;
                case 4: cache[4] = (seq_t)331; break;
                case 5: cache[5] = (seq_t)773; break;
                case 6: cache[6] = (seq_t)1782;break;
                default:
                    cache[i] = 2 * cache[i-1]
                               - cache[i-2]
                               + 3 * cache[i-3]
                               + cache[i-4]
                               + 2 * cache[i-5]
                               + 2 * cache[i-6];
            }
        }
        last_computed = n;
    }

    return cache[n];
}
