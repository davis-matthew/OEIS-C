/*
 * A298680  Start with the triangle with 4 markings of the Shield tiling and
 *          recursively apply the substitution rule. a(n) is the number of
 *          triangles with 6 markings after n iterations.
 * https://oeis.org/A298680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3*a(n-1) + 5*a(n-2) - 9*a(n-3) + 2*a(n-4)
 * (n>3), with initial values a(0)=0, a(1)=0, a(2)=4, a(3)=12.
 * This implements OEIS A298680.
 */

#include <stdlib.h>

static seq_t *cache   = NULL; /* cache of computed terms */
static idx_t  cap     = 0;    /* allocated size of cache   */
static idx_t  last   = 3;     /* highest index already computed */

/* Return nth term of OEIS A298680. */
seq_t a(idx_t n)
{
    /* Grow the cache if necessary to hold indices up to n. */
    if (cap <= n) {
        idx_t new_cap = cap == 0 ? 4 : cap;
        while (new_cap <= n) new_cap <<= 1;

        void *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure */
        cache = (seq_t *)tmp;
        cap   = new_cap;

        /* Initialize the base entries. */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)12;
        last = 3;
    }

    /* Compute any missing terms up to n. */
    if (last < n) {
        for (idx_t i = last + 1; i <= n; ++i) {
            cache[i] = (3 * cache[i-1]) + (5 * cache[i-2])
                     - (9 * cache[i-3]) + (2 * cache[i-4]);
        }
        last = n;
    }

    return cache[n];
}
