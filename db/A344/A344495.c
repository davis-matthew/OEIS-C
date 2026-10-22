/*
 * A344495  a(0)=1; for n>0 a(n)=(a(n-1) + n) * n if n is odd, a(n-1)*n + n
 *          otherwise.
 * https://oeis.org/A344495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n>0, if n is odd : a(n) = (a(n-1) + n) * n if n is
 *            even : a(n) = a(n-1) * n + n (OEIS A344495)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* allocated capacity */
    static size_t len   = 0;          /* number of elements stored (≤ cap) */

    /* initialise cache */
    if (cache == NULL)
    {
        cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (cache == NULL)
            return (seq_t)0;        /* allocation failure – unlikely */
        cache[0] = (seq_t)1;
        len = 1;
    }

    /* grow cache if needed */
    if ((size_t)n >= cap)
    {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL)
            return (seq_t)0;        /* allocation failure – unlikely */
        cache = tmp;
        cap = new_cap;
    }

    /* compute new values */
    for (size_t i = len; i <= (size_t)n; ++i)
    {
        seq_t prev = cache[i - 1];
        if (i & 1U)   /* odd i */
            cache[i] = (prev + (seq_t)i) * (seq_t)i;
        else          /* even i */
            cache[i] = prev * (seq_t)i + (seq_t)i;
    }

    len = (size_t)n + 1;
    return cache[n];
}
