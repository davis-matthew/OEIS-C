/*
 * A396344  Expansion of x * (3+4*x-7*x^2-6*x^3-x^4-4*x^5+3*x^6+2*x^7)/((1-x)
 *          * (1+x) * (1-3*x^2-x^4+x^6)).
 * https://oeis.org/A396344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) - 2*a(n-4) - 2*a(n-6) + a(n-8); offset 1
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial terms for indices 1..8 */
    static const seq_t init[9] = {0,
        3, 4, 5, 10, 13, 28, 39, 86};

    /* Cached values up to the largest index seen so far. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0; /* allocated capacity (index of last element) */
    static idx_t cache_size = 0;/* highest computed index */

    /* Ensure cache can hold index n */
    if (n > cache_cap) {
        idx_t new_cap = (cache_cap < 8) ? 8 : cache_cap;
        while (new_cap < n) {
            new_cap += new_cap;                /* double size */
            if (new_cap < n) new_cap = n;      /* safety */
        }
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed, return 0 as safe fallback */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values up to n */
    for (idx_t k = cache_size + 1; k <= n; ++k) {
        if (k <= 8) {
            cache[k] = init[k];
        } else {
            cache[k] = 4 * cache[k - 2]
                     - (2 * cache[k - 4])
                     - (2 * cache[k - 6])
                     + cache[k - 8];
        }
    }
    cache_size = n;
    return cache[n];
}
