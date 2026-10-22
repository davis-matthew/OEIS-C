/*
 * A398728  Upper (1/2, 3) midsequence of triangular numbers (A000217) and
 *          tetrahedral numbers (A000330); see Comments.
 * https://oeis.org/A398728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 7*a(n-2) + 8*a(n-3) - 7*a(n-4) + 4*a(n-5) -
 *            a(n-6); Initial values: a(0)=0, a(1)=4, a(2)=17, a(3)=45,
 *            a(4)=95, a(5)=173
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial values of the sequence (index 0..5) */
    static const seq_t init[6] = {0, 4, 17, 45, 95, 173};

    /* cache of computed terms */
    static seq_t *cache   = NULL;   /* storage for all computed terms   */
    static idx_t  len     = 0;      /* number of computed terms (valid indices) */
    static idx_t  capacity = 0;     /* allocated storage capacity            */

    /* Initialise the cache on the first call */
    if (!cache) {
        /* allocate space for at least 6 terms */
        capacity = 8;
        cache = (seq_t *)malloc((size_t)capacity * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed – return 0 as sentinel */

        /* store the first six initial values */
        for (idx_t i = 0; i < 6; ++i) cache[i] = init[i];
        len = 6;
    }

    /* If the requested index is already available, return it */
    if (n < len) return cache[n];

    /* Ensure the cache is large enough to hold index n */
    if (n >= capacity) {
        idx_t new_cap = capacity ? capacity : 8;
        while (new_cap <= n) new_cap <<= 1;   /* double until big enough */

        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed – sentinel */
        cache   = tmp;
        capacity = new_cap;
    }

    /* Compute missing terms up to index n */
    for (idx_t i = len; i <= n; ++i) {
        if (i < 6) {
            cache[i] = init[i];
        } else {
            cache[i] = (4  * cache[i-1])
                     - (7  * cache[i-2])
                     + (8  * cache[i-3])
                     - (7  * cache[i-4])
                     + (4  * cache[i-5])
                     -      cache[i-6];
        }
    }

    /* Update the length of computed terms */
    len = n + 1;

    return cache[n];
}
