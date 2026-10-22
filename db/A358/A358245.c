/*
 * A358245  Number of n-regular, N_0-weighted pseudographs on 2 vertices with
 *          total edge weight 5, up to isomorphism.
 * https://oeis.org/A358245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) for n >= 11 with initial values:
 *            a(1)=1, a(2)=6, a(3)=17, a(4)=36, a(5)=59, a(6)=87, a(7)=114,
 *            a(8)=145, a(9)=173, a(10)=205
 * Range:     u8 n <= 11, u16 n <= 2187, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage */
seq_t a(idx_t n)
{
    /* Zero based index is not used – sequence starts at 1 */
    if (n == 0) return 0;      /* safety for bad calls */

    /* static cache */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;      /* size allocated */
    static idx_t max_computed = 0;   /* highest n computed */

    /* Ensure array big enough */
    if (cache == NULL) {
        /* start with a moderate size */
        size_t cap = 32;
        while (cap <= n) cap <<= 1;          /* double until enough */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache_cap = (idx_t)cap;
    } else if (n >= cache_cap) {
        size_t new_cap = cache_cap << 1;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = (idx_t)new_cap;
    }

    /* Compute missing values up to n */
    if (n > max_computed) {
        /* initial values at indices 1..10 */
        static const seq_t init_vals[11] = {
            (seq_t)0,   /* index 0 unused */
            (seq_t)1,   (seq_t)6,   (seq_t)17,  (seq_t)36,
            (seq_t)59,  (seq_t)87,  (seq_t)114, (seq_t)145,
            (seq_t)173, (seq_t)205
        };

        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i <= 10) {
                cache[i] = init_vals[i];
            } else {
                cache[i] = cache[i-1] + cache[i-2] - cache[i-3];
            }
        }
        max_computed = n;
    }

    return cache[n];
}
