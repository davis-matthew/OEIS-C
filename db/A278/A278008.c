/*
 * A278008  Number of n X 2 0..2 arrays with every element plus 1 mod 3 equal
 *          to some element at offset (-1,-1) (-1,0) (-1,1) (0,-1) (0,1) or
 *          (1,0), with upper left element zero.
 * https://oeis.org/A278008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A278008: Number of n×2 0..2 arrays with the stated property
   Recurrence:   a(1) = 0, a(2) = 6
                 a(n) = 5*a(n-1) + 8*a(n-2)  for n ≥ 3   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store computed values up to the current
       maximum requested n.  index 0 is unused.                     */
    static seq_t *cache = NULL;      /* array of a(1..max)   */
    static idx_t cache_nmax = 0;      /* highest n computed   */
    static size_t cache_cap = 0;      /* allocated size (≥ n+1) */

    /* initialise cache if first time */
    if (cache == NULL) {
        /* allocate space for indices 0..2 (index 0 unused) */
        cache_cap = 3;                     /* indices 0,1,2   */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0;                      /* unused          */
        cache[1] = 0;                      /* a(1) = 0       */
        cache[2] = 6;                      /* a(2) = 6       */
        cache_nmax = 2;
    }

    /* if requested n is already known return it */
    if (n <= cache_nmax)
        return cache[n];

    /* ensure the storage is large enough for indices up to n */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap <= (size_t)n + 1)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) /* allocation failed – abort silently, but
                     sequence not required beyond current n   */
            return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values from cache_nmax+1 up to n */
    for (idx_t i = cache_nmax + 1; i <= n; ++i) {
        cache[i] = cache[i-1] * (seq_t)5 + cache[i-2] * (seq_t)8;
    }
    cache_nmax = n;

    return cache[n];
}
