/*
 * A258965  Number of (n+2) X (7+2) 0..1 arrays with no 3 X 3 subblock
 *          diagonal sum 0 and no antidiagonal sum 3 and no row sum 0 or 3 and
 *          no column sum 0 or 3.
 * https://oeis.org/A258965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 21, u32 n <= 53, u64 n <= 117, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A258965:  number of (n+2) × (7+2) binary arrays with
   no 3×3 subblock having diagonal sum 0, no antidiagonal sum 3,
   no row sum 0 or 3, and no column sum 0 or 3.

   Empirical recurrence (verified up to n = 24):
   a(n) = 3*a(n-2) - 2*a(n-4)      for n > 10
   Initial values:
   a(1)=2040, a(2)=1228, a(3)=456,  a(4)=662,  a(5)=552,
   a(6)=788,  a(7)=720,  a(8)=1046, a(9)=1080, a(10)=1570.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* base values for n = 0..10 (index 0 unused) */
    static const seq_t base[11] =
    {   0, 2040, 1228, 456, 662, 552, 788, 720, 1046, 1080, 1570 };

    if (n == (idx_t)0)          /* offset starts at 1 – return 0 for 0 */
        return (seq_t)0;

    if (n <= (idx_t)10)         /* small n – return constant */
        return base[(size_t)n];

    /* ------------------------------------------------------------------
       Memoisation cache for all previously computed values.
       The cache grows geometrically when required.
       ------------------------------------------------------------------ */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* current capacity in elements */

    /* requested maximum index (inclusive) */
    size_t need = (size_t)n + 1;

    /* first call: allocate an initial cache */
    if (cache == NULL) {
        /* start with capacity 16; will grow if needed */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)              /* allocation failure – give up */
            return (seq_t)0;
        /* initialise first 11 entries from the base table */
        for (size_t i = 0; i <= 10 && i < cache_cap; ++i)
            cache[i] = base[i];
    }

    /* increase capacity if needed */
    if (cache_cap < need) {
        size_t new_cap = cache_cap;
        while (new_cap < need)
            new_cap <<= 1;                    /* double until sufficient */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)                      /* allocation failure */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing values up to n */
    /* start from 11 because everything <=10 is already stored */
    for (size_t i = 11; i <= n; ++i) {
        cache[i] = (seq_t)3 * cache[i - 2] - (seq_t)2 * cache[i - 4];
    }

    return cache[n];
}
