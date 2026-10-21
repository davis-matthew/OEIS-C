/*
 * A282484  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 478", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A282484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 16*a(n-4) - 16*a(n-6) + p_n where p_n =
 *            1,3,5,12,2,-1,-5,0,0,0,8 for n=0…10, otherwise p_n = 0
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Small constant array for the numerator coefficients (up to n = 10). */
    static const int p[11] = {1, 3, 5, 12, 2, -1, -5, 0, 0, 0, 8};

    /* Memoised cache of computed values.  The cache is expanded on demand. */
    static seq_t *cache = NULL;
    static idx_t  last  = 6;               /* last index already computed   */
    static size_t cap   = 0;               /* current allocated capacity   */

    /* Helper to initialise the first 7 terms (0 … 6).  These are fixed. */
    if (cache == NULL) {
        /* Allocate a small initial capacity. */
        cap   = 16;
        cache = seq_alloc(cap);

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)15;
        cache[4] = (seq_t)24;
        cache[5] = (seq_t)62;   /* a(5) = a(3) + 16*a(1) - 1   */
        cache[6] = (seq_t)99;   /* a(6) = a(4) + 16*a(2) - 16*a(0) - 5 */
    }

    /* Return already computed value. */
    if (n <= last)
        return cache[n];

    /* Grow the cache if necessary. */
    if (n >= cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap <= n)
            new_cap *= 2;
        seq_t *tmp = seq_realloc(cache, cap, new_cap);
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = last + 1; i <= n; ++i) {

        /* For i >= 7 the recurrence is homogeneous except for p_n. */
        seq_t val = cache[i - 2]            /* a(i-2)  */
                 + (seq_t)16 * cache[i - 4];/* 16*a(i-4) */

        /* p_i is non‑zero only for i = 10 in this range. */
        if (i <= 10)
            val += (seq_t)p[i];

        /* Subtract 16*a(i-6).  The subtraction in unsigned arithmetic is safe
           because the sequence is strictly increasing and the term is larger
           than 16*a(i-6).  */
        val -= (seq_t)16 * cache[i - 6];

        cache[i] = val;
    }

    last = n;
    return cache[n];
}
