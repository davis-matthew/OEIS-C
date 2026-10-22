/*
 * A332185  a(n) = 8*(10^(2n+1)-1)/9 - 3*10^n.
 * https://oeis.org/A332185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A332185
   a(0) = 5
   a(1) = 858
   a(2) = 88588
   a(n) = 111*a(n-1) - 1110*a(n-2) + 1000*a(n-3)  for n >= 3
*/

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* dynamically sized array of sequence values */
    static size_t cap = 0;         /* current capacity of the array   */
    static size_t computed_upto = 0; /* maximum index that has been computed */

    /* Ensure the array is large enough for index n */
    size_t needed = (size_t)n;
    if (needed < 2) needed = 2;    /* at least 3 elements for the base values */
    ensure_cache(needed, &cache, &cap);

    /* Base cases – computed once on the first call */
    if (computed_upto < 2)
    {
        cache[0] = 5;
        cache[1] = 858;
        cache[2] = 88588;
        computed_upto = 2;
    }

    /* Compute any missing values in increasing order */
    size_t start = computed_upto + 1;
    if (start < 3) start = 3;      /* start with the first recurrence term */

    for (size_t i = start; i <= (size_t)n; ++i)
    {
        cache[i] = 111 * cache[i-1] - 1110 * cache[i-2] + 1000 * cache[i-3];
    }

    computed_upto = (size_t)n > computed_upto ? (size_t)n : computed_upto;

    return cache[n];
}
