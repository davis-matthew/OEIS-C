/*
 * A358587  Number of n-node rooted trees of height equal to the number of
 *          internal (non-leaf) nodes.
 * https://oeis.org/A358587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 62, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A358587: Number of n-node rooted trees whose height equals the number
   of internal (non-leaf) nodes.
   Recurrence (valid for n > 7):
     a(n) = 5*a(n-1) - 7*a(n-2) - a(n-3) + 8*a(n-4) - 4*a(n-5)
   Initial values:
     a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=1, a(6)=4, a(7)=14
*/

#include <stdlib.h>   /* for malloc and realloc */

seq_t a(idx_t n)
{
    /* ==== Setup initial values (indices 1..7) ==== */
    static const seq_t init[8] = {0, 0, 0, 0, 0, 1, 4, 14};

    if (n <= 7)                                 /* direct lookup */
        return init[n];

    /* ==== Static memoisation cache ==== */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;        /* allocated size (bytes) */
    static idx_t last_computed = 0;     /* highest index computed */

    /* ---- Allocate or grow cache if needed ---- */
    if (cache == NULL) {
        cache_cap = 8;                       /* cover indices 0..7 (unused 0) */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 1; i <= 7; ++i)
            cache[i] = init[i];
        last_computed = 7;
    }
    if ((size_t)n + 1 > cache_cap) {          /* need more space */
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)                       /* allocation failure */
            return 0;                          /* undefined, but defensive */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* ---- Compute missing values up to n ---- */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* recurrence calculation; use the fully opaque seq_t type */
        seq_t val = 5 * cache[i - 1] - 7 * cache[i - 2] -
                    cache[i - 3] + 8 * cache[i - 4] - 4 * cache[i - 5];
        cache[i] = val;
    }
    last_computed = n;
    return cache[n];
}
