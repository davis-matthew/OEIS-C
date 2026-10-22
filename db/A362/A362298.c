/*
 * A362298  Number of tilings of a 4 X n rectangle using dominos and 2 X 2
 *          right triangles.
 * https://oeis.org/A362298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 18*a(n-2) - 48*a(n-3) - 42*a(n-4) + 99*a(n-5)
 *            /* Initial values: a(0)=1, a(1)=1, a(2)=19, a(3)=55, a(4)=472
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Small table of initial terms */
    static const seq_t init[5] = { (seq_t)1, (seq_t)1, (seq_t)19,
                                   (seq_t)55, (seq_t)472 };

    /* Return an initial term directly for n < 5 */
    if (n < 5) return init[n];

    /* Static memoization state */
    static seq_t *cache      = NULL;   /* array holding already computed a[i] */
    static size_t  cap       = 0;      /* allocated size of cache   */
    static idx_t   max_comp  = 4;      /* highest index computed so far */
    static int     initialized = 0;    /* whether base terms have been written */

    /* Ensure the cache is large enough */
    if (cache == NULL || cap <= n) {
        size_t new_cap = (cap == 0) ? n + 1 : cap;
        while (new_cap <= n) new_cap <<= 1;        /* grow to > n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                        /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Populate the initial values once */
    if (!initialized) {
        cache[0] = init[0];
        cache[1] = init[1];
        cache[2] = init[2];
        cache[3] = init[3];
        cache[4] = init[4];
        initialized = 1;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        seq_t term = 4 * cache[i - 1] + 18 * cache[i - 2];
        term = term - 48 * cache[i - 3];
        term = term - 42 * cache[i - 4];
        term = term + 99 * cache[i - 5];
        cache[i] = term;
    }
    max_comp = n;

    return cache[n];
}
