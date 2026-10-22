/*
 * A332647  a(n) = 2*a(n-1) + a(n-3) with a(0) = 3, a(1) = 2, a(2) = 4.
 * https://oeis.org/A332647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-3), a(0)=3, a(1)=2, a(2)=4
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 56, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;  /* current allocation size */
    static idx_t  last  = 0;  /* highest index computed so far */

    /* initialize cache with the first three terms */
    if (cache == NULL) {
        cap   = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        last    = 2;
    }

    /* return already computed value early */
    if (n <= last)
        return cache[n];

    /* grow allocation if necessary */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap < 16 ? 16 : cap;
        while ((size_t)n + 1 > newcap)
            newcap <<= 1;  /* double until enough */
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap   = newcap;
    }

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] * (seq_t)2 + cache[i - 3];
    }
    last = n;

    return cache[n];
}
