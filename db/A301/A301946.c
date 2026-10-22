/*
 * A301946  Number of nX3 0..1 arrays with every element equal to 1, 2 or 3
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - 2*a(n-3), a(1)=1, a(2)=5, a(3)=20
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    if (n < 1)          /* undefined below the offset */
        return (seq_t)0;

    /* Static cache of computed values */
    static seq_t *cache = NULL;   /* array of seq_t */
    static size_t count = 0;      /* number of terms already stored (a(1) .. a(count)) */
    static size_t cap   = 0;      /* allocated capacity of cache (in terms) */

    /* initialize cache with first three terms on first call */
    if (count == 0) {
        cap   = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();            /* memory allocation failure */
        cache[0] = (seq_t)1;            /* a(1) */
        cache[1] = (seq_t)5;            /* a(2) */
        cache[2] = (seq_t)20;           /* a(3) */
        count = 3;
    }

    /* Expand cache up to the requested index */
    if ((size_t)n > count) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* memory allocation failure */
        cache = tmp;
        cap   = new_cap;
        /* Compute new terms using the recurrence */
        for (size_t i = count; i < n; ++i) {
            /* i corresponds to term a(i+1) */
            /* a(i+1) = 4*a(i) + a(i-1) - 2*a(i-2) */
            cache[i] = (seq_t)4 * cache[i-1]
                     + cache[i-2]
                     - (seq_t)2 * cache[i-3];
        }
        count = n;
    }

    /* Return the requested value (index 1 is at cache[0]) */
    return cache[(size_t)(n - 1)];
}
