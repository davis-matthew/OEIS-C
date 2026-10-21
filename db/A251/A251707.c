/*
 * A251707  6-step Fibonacci sequence starting with (0,0,0,1,0,0).
 * https://oeis.org/A251707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+6) = a(n)+a(n+1)+a(n+2)+a(n+3)+a(n+4)+a(n+5); Initial
 *            values: a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=0, a(5)=0
 * Range:     u8 n <= 14, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;          /* allocated array of terms   */
    static size_t cap = 0;               /* current allocation size   */
    static idx_t stored_len = 0;         /* number of terms stored    */

    /* initialise cache if this is the first call */
    if (cache == NULL) {
        cap = 64;                          /* initial capacity          */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        /* seed the first six values                     */
        cache[0] = 0; cache[1] = 0; cache[2] = 0;
        cache[3] = 1; cache[4] = 0; cache[5] = 0;
        stored_len = 6;
    }

    /* if the requested index is already known, return it */
    if (n < stored_len)
        return cache[(size_t)n];

    /* extend the allocated array if necessary */
    while ((size_t)n + 1 > cap) {
        size_t new_cap = cap * 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* compute successive terms up to the requested index */
    for (idx_t i = stored_len; i <= n; ++i) {
        seq_t sum = 0;
        /* sum of the previous six terms */
        sum += cache[i - 1];
        sum += cache[i - 2];
        sum += cache[i - 3];
        sum += cache[i - 4];
        sum += cache[i - 5];
        sum += cache[i - 6];
        cache[i] = sum;
    }
    stored_len = n + 1;

    return cache[(size_t)n];
}
