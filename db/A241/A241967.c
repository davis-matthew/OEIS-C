/*
 * A241967  Number of length 4+3 0..n arrays with no consecutive four elements
 *          summing to more than 2*n.
 * https://oeis.org/A241967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) + 56*a(n-5)
 *            - 28*a(n-6) + 8*a(n-7) - a(n-8); with initial values a(1)=57,
 *            a(2)=775, a(3)=5211, a(4)=23320, a(5)=80132, a(6)=228826,
 *            a(7)=569874, a(8)=1277427
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 28, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Computes the n-th term of OEIS sequence A241967.
 *  n >= 1.
 *  On subsequent calls the value is memoised for future use,
 *  so the function works in any call order.
 */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined, but return 0 to avoid bad access */
        return 0;

    /* Initial eight terms */
    static const seq_t init[8] = {
        57,     775,    5211,   23320,
        80132, 228826, 569874, 1277427
    };

    /* State for memoisation */
    static seq_t *cache   = NULL;   /* array of cached terms */
    static size_t cache_len = 0;    /* number of terms cached so far */
    static size_t cache_cap = 0;    /* allocated capacity of cache */

    /* Convert n (1‑based) to 0‑based index for array */
    size_t idx = (size_t)n;

    /* Grow cache if necessary */
    if (idx > cache_len) {
        /* Determine new capacity */
        size_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap < idx)
            new_cap = new_cap * 2;       /* grow geometrically */

        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)                     /* allocation failed */
            return 0;
        cache = new_cache;
        cache_cap = new_cap;

        /* Compute missing terms */
        for (size_t i = cache_len; i < idx; ++i) {
            if (i < 8) {
                cache[i] = init[i];
            } else {
                cache[i] = 8  * cache[i-1]
                           -28 * cache[i-2]
                           +56 * cache[i-3]
                           -70 * cache[i-4]
                           +56 * cache[i-5]
                           -28 * cache[i-6]
                           +8  * cache[i-7]
                           -  cache[i-8];
            }
        }
        cache_len = idx;
    }

    return cache[idx-1];   /* a(1) is stored at cache[0] */
}
