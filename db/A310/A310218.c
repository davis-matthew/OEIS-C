/*
 * A310218  Coordination sequence Gal.5.20.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-7) - a(n-14) (for n > 14) Initial values: a(0)=1,
 *            a(1)=3, a(2)=7, a(3)=12, a(4)=14, a(5)=21, a(6)=25, a(7)=26,
 *            a(8)=29, a(9)=35, a(10)=40, a(11)=40, a(12)=49, a(13)=53,
 *            a(14)=52
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */
#include <string.h>   /* memcpy */

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;          /* pointer to array */
    static size_t cached_size = 0;       /* allocated size (in elements) */
    static idx_t  last_computed = 0;     /* highest index computed so far */

    /* Initial values for indices 0..14 */
    static const seq_t init_vals[15] = {
        1, 3, 7, 12, 14, 21, 25, 26, 29, 35,
        40, 40, 49, 53, 52
    };

    /* Allocate and initialise cache on first call */
    if (cache == NULL) {
        cached_size   = 15;
        cache         = (seq_t *)malloc(cached_size * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;                 /* out of memory – unlikely here */
        memcpy(cache, init_vals, 15 * sizeof(seq_t));
        last_computed = 14;
    }

    /* If requested index already cached, return it */
    if ((size_t)n < cached_size)
        return cache[(size_t)n];

    /* Grow cache to accommodate new index n */
    size_t new_size = (size_t)n + 1;
    if (new_size > cached_size) {
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) {
            /* realloc failed – free old cache and return 0 */
            free(cache);
            cache = NULL;
            cached_size = 0;
            last_computed = 0;
            return (seq_t)0;
        }
        cache   = tmp;
        cached_size = new_size;
    }

    /* Compute successive terms until n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val = (seq_t)2 * cache[(size_t)(i - 7)] - cache[(size_t)(i - 14)];
        cache[(size_t)i] = val;
    }
    last_computed = n;

    return cache[(size_t)n];
}
