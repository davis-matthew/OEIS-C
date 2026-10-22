/*
 * A370767  Number of signed permutations of length n+1 with adjacent elements
 *          differing by more than 1 and whose first element is 1.
 * https://oeis.org/A370767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=3, a(3)=17 For n>=4: a(n) = (1+2n)a(n-1) +
 *            (3-2n)a(n-2) + (5-2n)a(n-3) + (-4+2n)a(n-4) Positive multipliers
 *            are added, negative multipliers are subtracted from the sum. All
 *            arithmetic is performed in the type seq_t which is an unsigned
 *            integer (or arbitrary‑precision unsigned...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements currently stored (indices 0..cache_size-1) */
    static size_t cache_cap   = 0;   /* allocated array capacity */

    /* Return already known value */
    if (n < cache_size)
        return cache[n];

    /* Ensure enough allocated space */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 16) new_cap = 16;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure – per requirement */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base cases if not yet done */
    if (cache_size < 4) {
        /* Compute all base entries that are needed for this call */
        size_t limit = 4;
        if (limit > (size_t)n + 1)
            limit = (size_t)n + 1;
        for (size_t i = cache_size; i < limit; ++i) {
            if (i == 0) cache[0] = (seq_t)1;
            else if (i == 1) cache[1] = (seq_t)1;
            else if (i == 2) cache[2] = (seq_t)3;
            else /* i == 3 */ cache[3] = (seq_t)17;
        }
        cache_size = limit;
    }

    /* If we already have all requested values, return */
    if (n < cache_size)
        return cache[n];

    /* Compute missing values using the recurrence */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        /* For i < 4 the base values have already been stored. */
        if (i < 4) continue;

        seq_t coeff1      = (seq_t)(1 + 2 * i);        /* (1+2n)  */
        seq_t coeff2_abs  = (seq_t)(2 * i - 3);        /* |3-2n|  */
        seq_t coeff3_abs  = (seq_t)(2 * i - 5);        /* |5-2n|  */
        seq_t coeff4      = (seq_t)(2 * i - 4);        /* (-4+2n) */

        seq_t term1 = coeff1 * cache[i - 1];
        seq_t term2 = coeff2_abs * cache[i - 2];
        seq_t term3 = coeff3_abs * cache[i - 3];
        seq_t term4 = coeff4 * cache[i - 4];

        seq_t val = term1 + term4 - term2 - term3;
        cache[i] = val;
    }

    cache_size = (size_t)n + 1;
    return cache[n];
}
