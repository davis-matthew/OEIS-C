/*
 * A302769  a(n) = 2*((2*n-1)*a(n-1) - (n-2)!), with a(1) = 4, n > 1.
 * https://oeis.org/A302769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*((2*n-1)*a(n-1) - (n-2)!)   (a(1)=4, n>1)
 *
 * The function iteratively builds the sequence and keeps a cache of
 * already computed values.  Only the previous factorial ( (i-1)! )
 * needed for the recurrence is stored, so no huge factorial table
 * is required.  The implementation works for any sequence value
 * type `seq_t` (unsigned integer, 128‑bit, or arbitrary‑precision)
 * and any index type `idx_t` (uint64_t).  All operations are
 * performed using the standard C operators allowed by the
 * specification.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Static state to allow arbitrary call order */
    static seq_t *a_cache = NULL;      /* Cache for a[1]..a[max_n] */
    static size_t a_cap  = 0;          /* Current capacity of a_cache   */
    static idx_t  max_n  = 0;          /* Highest n computed so far     */
    static seq_t  fac    = 0;          /* Holds (i-1)! during iteration */
    static int    init   = 0;          /* Have we performed initialization? */

    /* Primitive helpers for resizing the cache */
    if (!init) {
        /* No previous data: allocate minimal cache and seed */
        a_cap = 8;                    /* initial capacity   */
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        if (!a_cache) return 0;       /* out of memory; caller must handle */

        /* initialise a(1) */
        a_cache[1] = (seq_t)4;
        max_n = 1;
        fac   = (seq_t)1;             /* 0! = 1, needed for the next step */
        init  = 1;
    }

    /* Grow the cache if necessary */
    while (a_cap <= n) {
        size_t new_cap = a_cap * 2;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* out of memory */
        a_cache = tmp;
        a_cap   = new_cap;
    }

    /* If the value has already been computed, just return it */
    if (max_n >= n)
        return a_cache[n];

    /* Compute all missing values up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* (2*i - 1) is small, cast to seq_t for multiplication */
        seq_t temp  = (seq_t)(2 * i - 1) * a_cache[i - 1];
        seq_t term  = temp - fac;          /* (i-2)! is stored in fac    */
        a_cache[i]  = (seq_t)2 * term;     /* a(i) = 2*((2i-1)a(i-1) - (i-2)!) */
        fac = fac * (i - 1);               /* advance factorial for next i */
    }

    max_n = n;
    return a_cache[n];
}
