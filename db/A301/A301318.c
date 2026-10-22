/*
 * A301318  a(n) = sqrt(A299921(n)).
 * https://oeis.org/A301318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 39, u64 n <= 77, u128 n <= 152,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A301318: a(n) = sqrt(A299921(n))
    Generating function: x*(1+2x+3x^2-2x^3-x^4)/(1-6x^3+x^6)
    Recurrence (for n ≥ 7): a(n) = 6*a(n‑3) – a(n‑6)
    Initial values: a(1)=0,a(2)=1,a(3)=2,a(4)=3,a(5)=4,a(6)=11
*/

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;      /* array indexed by n (1‑based) */
    static idx_t cache_size = 0;     /* allocated size (>= n+1) */
    static idx_t max_computed = 0;   /* highest n already computed */

    /* ensure capacity for index n */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = new seq_t[new_size];
        if (cache) {
            for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
            delete[] cache;
        }
#else
        new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (cache) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }
#endif
        cache = new_cache;
        cache_size = new_size;
    }

    /* fill cache up to n if needed */
    if (n > max_computed) {
        /* known values for n = 1..6 */
        static const seq_t init[6] = { 0, 1, 2, 3, 4, 11 };
        idx_t i;
        for (i = max_computed + 1; i <= n; ++i) {
            if (i <= 6) {
                cache[i] = init[i - 1];
            } else {
                /* a(i) = 6*a(i-3) - a(i-6) */
                cache[i] = (seq_t)6 * cache[i - 3] - cache[i - 6];
            }
        }
        max_computed = n;
    }

    return cache[n];
}
