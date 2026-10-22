/*
 * A382494  a(n) = Sum_{k=0..floor(n/2)} binomial(k+2,2) *
 *          binomial(2*k,2*n-4*k).
 * https://oeis.org/A382494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(k+2,2)*binomial(2k,2n-4k)
 *            Recurrence (order 18) used for efficient computation: a(n) =
 *            6*a(n-2)+6*a(n-3)-15*a(n-4)-18*a(n-5)+5*a(n-6)+12*a(n-7)
 *            -3*a(n-8)+32*a(n-9)+12*a(n-10)-6*a(n-11)-4*a(n-12)
 *            +18*a(n-13)-33*a(n-14)+26*a(n-15)-15*a(n-16)+6*a(n-17)-a(n-18)
 *            I...
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 70, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;  /* number of values currently stored      */
    static size_t cache_cap  = 0;  /* allocated capacity of the cache array  */

    /* Initial terms for n = 0 … 17 */
    static const seq_t init_vals[18] = {
        1, 0, 3, 3, 6, 36, 16, 150, 165, 430,
        1071, 1365, 4453, 6258, 14841, 29169,
        49941, 115356
    };

    /* --- 1. Initialise or grow the cache if needed -------------------- */
    if (cache == NULL) {
        /* First allocation: at least enough for the first 18 terms or for n. */
        size_t first_cap = (size_t)(n + 1 > 18 ? n + 1 : 18);
        cache_cap = first_cap;
        cache_size = first_cap < 18 ? (idx_t)first_cap : 18;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failure – unlikely in normal use   */
        /* Copy the 18 known terms into the array                      */
        memcpy(cache, init_vals, cache_size * sizeof(seq_t));
    }

    if ((n + 1) > cache_cap) {
        /* Grow the storage – double the current capacity or just enough for n */
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < cache_cap * 2) new_cap = cache_cap * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;  /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* --- 2. Compute missing terms up to n ------------------------------ */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 18) {
            /* already initialised – nothing to do */
            continue;
        }
        seq_t val = 6 * cache[i - 2] + 6 * cache[i - 3]
                  - 15 * cache[i - 4] - 18 * cache[i - 5]
                  + 5  * cache[i - 6] + 12 * cache[i - 7]
                  - 3  * cache[i - 8] + 32 * cache[i - 9]
                  + 12 * cache[i -10] - 6  * cache[i -11]
                  - 4  * cache[i -12] + 18 * cache[i -13]
                  -33 * cache[i -14] + 26 * cache[i -15]
                  -15 * cache[i -16] + 6  * cache[i -17]
                  -     cache[i -18];
        cache[i] = val;
    }

    cache_size = n + 1;
    return cache[n];
}
