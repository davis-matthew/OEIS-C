/*
 * A400810  Size of Type C megastars of star triples in the Spectre aperiodic
 *          monotile tiling.
 * https://oeis.org/A400810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = 3*a(n-1) + 15*a(n-2) + a(n-3) (n > 3) Initial values:
 *            a(1) = 1, a(2) = 7, a(3) = 37
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* undefined for n == 0; safe guard */

    /* Static cache for computed values */
    static seq_t *cache    = NULL;   /* pointer to array of seq_t */
    static idx_t cache_cap = 0;      /* allocated capacity (elements) */
    static idx_t computed  = 0;      /* number of elements already filled */

    /* Grow the cache if necessary */
    if (cache_cap < n)
    {
        idx_t new_cap = cache_cap < 8 ? 8 : cache_cap;
        while (new_cap < n)
            new_cap *= 2;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values using the recurrence */
    for (idx_t i = computed + 1; i <= n; ++i)
    {
        if (i == 1)
            cache[0] = (seq_t)1;
        else if (i == 2)
            cache[1] = (seq_t)7;
        else if (i == 3)
            cache[2] = (seq_t)37;
        else
        {
            /* a(i) = 3*a(i-1) + 15*a(i-2) + a(i-3) */
            seq_t term = (seq_t)3 * cache[i - 2]
                       + (seq_t)15 * cache[i - 3]
                       +      cache[i - 4];
            cache[i - 1] = term;
        }
    }
    computed = n;

    return cache[n - 1];
}
