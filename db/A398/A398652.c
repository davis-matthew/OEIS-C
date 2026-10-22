/*
 * A398652  Upper (1, 1/2)-midsequence of triangular numbers (A000217) and
 *          tetrahedral numbers (A000330); see Comments.
 * https://oeis.org/A398652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A398652 Upper (1,1/2)-midsequence of triangular and tetrahedral numbers
   Recurrence: a(n) = 4*a(n-1) - 7*a(n-2) + 8*a(n-3) 
                 - 7*a(n-4) + 4*a(n-5) - a(n-6)
   initial values: a(0)=0, a(1)=2, a(2)=6, a(3)=13, a(4)=25, a(5)=43
*/
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache holds already computed terms */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* first call: initialise cache with the six first terms */
    if (cache == NULL) {
        cache_cap = 8;                         /* provide some spare space   */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;    /* allocation failure – unlikely  */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)13;
        cache[4] = (seq_t)25;
        cache[5] = (seq_t)43;
        cache_len = 6;
    }

    /* if the value is already computed, return it */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* grow the cache to accommodate n */
    if ((size_t)n >= cache_cap) {
        size_t needed = (size_t)n + 1;
        size_t new_cap = cache_cap;
        while (new_cap < needed) new_cap <<= 1;        /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;              /* allocation failure        */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute all missing terms up to n using the recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* compute term = (4*a(i-1)+8*a(i-3)+4*a(i-5)) -
                         (7*a(i-2)+7*a(i-4)+a(i-6)) */
        seq_t pos = 4 * cache[i - 1];
        pos += 8 * cache[i - 3];
        pos += 4 * cache[i - 5];

        seq_t neg = 7 * cache[i - 2];
        neg += 7 * cache[i - 4];
        neg += cache[i - 6];

        cache[i] = pos - neg;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
