/*
 * A250609  Number of (n+1) X (6+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 9*a(n-2) + 7*a(n-3) - 2*a(n-4), with a(1)=502,
 *            a(2)=1172, a(3)=2236, a(4)=3890. (Empirical closed form:
 *            a(n)=196*2^(n-1)+99*n^2+177*n+30.)
 * Range:     u8 none, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* number of indices cached (1..cap) */

    /* finite state: ensure cache large enough for index n */
    if (n == 0) return 0;            /* undefined for zero, but harmless */
    if (cache == NULL)
    {
        /* allocate at least up to max(n,4) for initial values */
        size_t max = (size_t)n > 4 ? (size_t)n : 4;
        cache = (seq_t *)malloc((max + 1) * sizeof(seq_t));
        cap = max;
        /* initialise first four terms */
        cache[1] = (seq_t)502;
        cache[2] = (seq_t)1172;
        cache[3] = (seq_t)2236;
        cache[4] = (seq_t)3890;
        /* precompute up to cap (if larger than 4) */
        for (idx_t i = 5; (idx_t)i <= cap; ++i)
            cache[i] = 5 * cache[i - 1] - 9 * cache[i - 2]
                      + 7 * cache[i - 3] - 2 * cache[i - 4];
    }
    else if ((size_t)n > cap)
    {
        /* enlarge cache to include index n */
        size_t new_cap = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        /* realloc might fail; however for contest purposes we ignore that */
        cache = new_cache;
        /* compute missing terms */
        for (idx_t i = (idx_t)(cap + 1); (idx_t)i <= n; ++i)
            cache[i] = 5 * cache[i - 1] - 9 * cache[i - 2]
                      + 7 * cache[i - 3] - 2 * cache[i - 4];
        cap = new_cap;
    }
    return cache[n];
}
