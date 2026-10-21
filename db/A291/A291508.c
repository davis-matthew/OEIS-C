/*
 * A291508  a(n) = (n!)^10 * Sum_{i=1..n} 1/i^10.
 * https://oeis.org/A291508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   pow10_of_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A291508
    a(n) = (n!)^10 * Sum_{i=1..n} 1/i^10,   offset 0
    Recurrence: a(0)=0, a(1)=1;
    a(n+1) = (n^10+(n+1)^10)*a(n) - n^20*a(n-1)   for n>0
*/
#include <stdlib.h>
#include "../utils/pow10_of_idx.h"

/* Cache for computed terms. */
static seq_t *a_cache = NULL;     /* a(n) values */
static idx_t  a_cache_size = 0;   /* number of elements stored */
static idx_t  a_cache_last = 0;   /* last index that has been computed */

/* The sequence function. */
seq_t a(idx_t n)
{
    /* Ensure the cache is large enough.  The first call gets a minimal
       allocation of size 2: a(0) and a(1). */
    if (a_cache == NULL) {
        a_cache_size = (n >= 1) ? n + 1 : 2;
        a_cache = (seq_t *)malloc(a_cache_size * sizeof(seq_t));
        if (!a_cache) return 0;          /* fallback on allocation failure   */
        a_cache[0] = (seq_t)0;           /* a(0) = 0                          */
        if (a_cache_size > 1)
            a_cache[1] = (seq_t)1;       /* a(1) = 1                          */
        a_cache_last = 1;
    }

    /* Grow the cache if the requested index exceeds current capacity. */
    if (n + 1 > a_cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;              /* fallback on allocation failure   */
        a_cache     = tmp;
        a_cache_size = new_size;         /* preserve existing values          */
    }

    /* If the value is already computed, return it. */
    if (n <= a_cache_last)
        return a_cache[(size_t)n];

    /* Compute new terms up to n using the recurrence. */
    for (idx_t i = a_cache_last + 1; i <= n; ++i) {
        if (i == 1) {
            a_cache[(size_t)1] = (seq_t)1;
            continue;
        }
        /* n = i-1 in recurrence expression */
        idx_t prev = i - 1;
        seq_t pow10_prev  = pow10_of_idx(prev);   /* (i-1)^10   */
        seq_t pow10_curr  = pow10_of_idx(i);      /*  i^10      */
        seq_t pow20_prev  = pow10_prev * pow10_prev;

        /* a(i) = ( (i-1)^10 + i^10 ) * a(i-1) - ( (i-1)^20 * a(i-2) )   */
        seq_t term1 = (pow10_prev + pow10_curr) * a_cache[(size_t)prev];
        seq_t term2 = pow20_prev * a_cache[(size_t)(prev - 1)];
        a_cache[(size_t)i] = term1 - term2;
    }
    a_cache_last = n;
    return a_cache[(size_t)n];
}
