/*
 * A270258  Number of 3X3X3 triangular 0..n arrays with some element plus some
 *          adjacent element totalling n exactly once.
 * https://oeis.org/A270258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+3*a(n-2)-8*a(n-3)-2*a(n-4)+12*a(n-5)
 *            -2*a(n-6)-8*a(n-7)+3*a(n-8)+2*a(n-9)-a(n-10) Initial values:
 *            a(1)=0, a(2)=66, a(3)=768, a(4)=4428, a(5)=14976, a(6)=42750,
 *            a(7)=96768, a(8)=203448, a(9)=376320, a(10)=668250
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 55, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Invalid or offset-less call: return 0 */
    if (n == 0) return (seq_t)0;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;

    /* If cache is empty, initialise it with the first ten terms */
    if (cache_size == 0) {
        cache_cap = 10;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        static const seq_t init_vals[10] = {
            (seq_t)0,      (seq_t)66,      (seq_t)768,
            (seq_t)4428,   (seq_t)14976,   (seq_t)42750,
            (seq_t)96768,  (seq_t)203448,  (seq_t)376320,
            (seq_t)668250
        };
        for (size_t i = 0; i < 10; ++i)
            cache[i] = init_vals[i];
        cache_size = 10;
    }

    /* Ensure capacity for the requested term */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – return 0 as safe fallback */
            free(cache);
            cache = NULL;
            cache_cap = cache_size = 0;
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If the value is already computed, return it immediately */
    if ((size_t)n <= cache_size)
        return cache[n-1];

    /* Compute any missing terms using the 10‑term recurrence */
    for (size_t idx = cache_size; idx < (size_t)n; ++idx) {
        seq_t val = 0;
        val += (seq_t)2  * cache[idx - 1];
        val += (seq_t)3  * cache[idx - 2];
        val -= (seq_t)8  * cache[idx - 3];
        val -= (seq_t)2  * cache[idx - 4];
        val += (seq_t)12 * cache[idx - 5];
        val -= (seq_t)2  * cache[idx - 6];
        val -= (seq_t)8  * cache[idx - 7];
        val += (seq_t)3  * cache[idx - 8];
        val += (seq_t)2  * cache[idx - 9];
        val -=          cache[idx -10];
        cache[idx] = val;
    }

    cache_size = (size_t)n;
    return cache[n-1];
}
