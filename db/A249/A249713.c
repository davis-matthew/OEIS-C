/*
 * A249713  Number of length 7+3 0..n arrays with every four consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining two terms.
 * https://oeis.org/A249713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) + 56*a(n-5)
 *            - 28*a(n-6) + 8*a(n-7) - a(n-8) (for n>8) Initial values:
 *            a(1)=72, a(2)=1177, a(3)=7056, a(4)=27113, a(5)=80360,
 *            a(6)=200489, a(7)=442144, a(8)=888465
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 35, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
#  include <algorithm>
#endif

seq_t a(idx_t n)
{
    /* Static cache for memoised values ----------------------------------- */
    static seq_t *cache = NULL;          /* array of cached a(i), i>=0   */
    static idx_t cache_cap = 0;          /* allocated capacity (max index) */
    static idx_t cache_max = 0;          /* highest index already computed */

    /* First initialisation ------------------------------------------------ */
    if (cache == NULL) {
        /* Allocate space for indices 0 … 8 (ignore 0) */
        cache_cap = 8;
#ifdef __cplusplus
        cache = new seq_t[cache_cap + 1];
#else
        cache = (seq_t *)calloc((size_t)(cache_cap + 1), sizeof(seq_t));
        if (!cache) abort();
#endif
        /* Store the 8 given initial values */
        cache[1] = (seq_t)72;
        cache[2] = (seq_t)1177;
        cache[3] = (seq_t)7056;
        cache[4] = (seq_t)27113;
        cache[5] = (seq_t)80360;
        cache[6] = (seq_t)200489;
        cache[7] = (seq_t)442144;
        cache[8] = (seq_t)888465;
        cache_max = 8;
    }

    /* a(0) is outside the offset – return 0 (undefined behaviour otherwise) */
    if (n == 0) return (seq_t)0;

    /* Ensure capacity to hold up to index n --------------------------------- */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap < n) {
            new_cap <<= 1;
            if (new_cap == 0) {          /* overflow guard */
                new_cap = n;
                break;
            }
        }
#ifdef __cplusplus
        /* Allocate new array and copy existing values */
        seq_t *tmp = new seq_t[new_cap + 1];
        for (idx_t i = 0; i <= cache_cap; ++i)
            tmp[i] = cache[i];
        delete [] cache;
        cache = tmp;
#else
        /* Reallocate using C realloc */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
#endif
        cache_cap = new_cap;
    }

    /* Compute values lazily up to n ---------------------------------------- */
    if (n > cache_max) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t val = (seq_t)0;
            /* Positive terms (increasing indices) */
            val += (seq_t)8   * cache[i-1];
            val += (seq_t)56  * cache[i-3];
            val += (seq_t)56  * cache[i-5];
            val += (seq_t)8   * cache[i-7];
            /* Negative terms (decreasing indices) */
            val -= (seq_t)28  * cache[i-2];
            val -= (seq_t)70  * cache[i-4];
            val -= (seq_t)28  * cache[i-6];
            val -=            cache[i-8];
            cache[i] = val;
        }
        cache_max = n;
    }

    return cache[n];
}
