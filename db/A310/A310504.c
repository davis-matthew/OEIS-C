/*
 * A310504  Coordination sequence Gal.6.329.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) * -
 *            2*a(n-6) + 2*a(n-7) - 2*a(n-8) + 2*a(n-9) - a(n-10), * for n >
 *            10, with a(0)=1, a(1)=4, a(2)=10, a(3)=16, * a(4)=20, a(5)=26,
 *            a(6)=32, a(7)=36, a(8)=42, a(9)=48, a(10)=52
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t max_computed = 0;  /* highest index that has a valid value */
    static size_t cache_cap   = 0;    /* allocated size of cache */

    /* The first 11 values are known a priori */
    const size_t init_size = 11;

    /* Make sure cache is large enough for the requested index */
    size_t required_cap = (size_t)n + 1;
    if (required_cap < init_size) required_cap = init_size;
    if (cache_cap < required_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, required_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure: return zero */
        cache = tmp;
        cache_cap = required_cap;
    }

    /* Initialize the cache with the first 11 terms once */
    if (max_computed == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)10;
        cache[3] = (seq_t)16;
        cache[4] = (seq_t)20;
        cache[5] = (seq_t)26;
        cache[6] = (seq_t)32;
        cache[7] = (seq_t)36;
        cache[8] = (seq_t)42;
        cache[9] = (seq_t)48;
        cache[10] = (seq_t)52;
        max_computed = 10;
    }

    /* Compute any missing values up to n */
    if ((size_t)n > max_computed) {
        for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)(2 * cache[i-1]
                               - 2 * cache[i-2]
                               + 2 * cache[i-3]
                               - 2 * cache[i-4]
                               + 2 * cache[i-5]
                               - 2 * cache[i-6]
                               + 2 * cache[i-7]
                               - 2 * cache[i-8]
                               + 2 * cache[i-9]
                               - cache[i-10]);
        }
        max_computed = (size_t)n;
    }

    return cache[(size_t)n];
}
