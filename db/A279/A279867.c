/*
 * A279867  Number of n X 3 0..2 arrays with no element unequal to a strict
 *          majority of its horizontal and vertical neighbors, with the
 *          exception of exactly two elements, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A279867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) -13*a(n-2) -3*a(n-3) +13*a(n-4) +11*a(n-5)
 *            +29*a(n-6) -49*a(n-7) -8*a(n-8) -38*a(n-9) +28*a(n-10)
 *            +8*a(n-11) +16*a(n-12) for n>18 Initial values (n=1..18):
 *            0,33,176,824,3199,11983,42296,143968,476675,1543667,4909096,
 *            15379744,47578031,145603087,441441920,1327480296,3963270819,
 *            117...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 38, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* External linkage required */
seq_t a(idx_t n)
{
    /* Return 0 for invalid indices */
    if (n < 1) return (seq_t)0;

    /* Static cache and bookkeeping */
    static seq_t *cache     = NULL;   /* 1‑based array of terms */
    static idx_t cached_n   = 0;      /* largest computed index */
    static size_t cache_cap = 0;      /* allocated capacity */

    /* Helper to ensure capacity */
    if (n + 1 > cache_cap) {
        size_t new_cap = n + 32;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* First call: fill known initial values */
    if (cached_n == 0) {
        /* Set up size to at least 18 */
        if (cache_cap < 19) {
            size_t new_cap = (cache_cap > 0 ? cache_cap : 32);
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[0] = (seq_t)0;            /* unused, sequence starts at 1 */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)33;
        cache[3] = (seq_t)176;
        cache[4] = (seq_t)824;
        cache[5] = (seq_t)3199;
        cache[6] = (seq_t)11983;
        cache[7] = (seq_t)42296;
        cache[8] = (seq_t)143968;
        cache[9] = (seq_t)476675;
        cache[10] = (seq_t)1543667;
        cache[11] = (seq_t)4909096;
        cache[12] = (seq_t)15379744;
        cache[13] = (seq_t)47578031;
        cache[14] = (seq_t)145603087;
        cache[15] = (seq_t)441441920;
        cache[16] = (seq_t)1327480296;
        cache[17] = (seq_t)3963270819;
        cache[18] = (seq_t)11757089555;
        cached_n = 18;
    }

    /* Compute any missing terms up to n */
    if (n > cached_n) {
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            seq_t val;
            if (i <= 18) {
                /* The initial values are known; already filled */
                val = cache[i];
            } else {
                /* Apply the recurrence */
                val = (seq_t)7   * cache[i-1]
                    - (seq_t)13  * cache[i-2]
                    - (seq_t)3   * cache[i-3]
                    + (seq_t)13  * cache[i-4]
                    + (seq_t)11  * cache[i-5]
                    + (seq_t)29  * cache[i-6]
                    - (seq_t)49  * cache[i-7]
                    - (seq_t)8   * cache[i-8]
                    - (seq_t)38  * cache[i-9]
                    + (seq_t)28  * cache[i-10]
                    + (seq_t)8   * cache[i-11]
                    + (seq_t)16  * cache[i-12];
            }
            cache[i] = val;
        }
        cached_n = n;
    }

    return cache[n];
}
