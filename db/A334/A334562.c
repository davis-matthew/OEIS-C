/*
 * A334562  E.g.f.: exp(-(x + x^2 + x^3)).
 * https://oeis.org/A334562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 14, i64 n <= 24, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A334562 (OEIS ID: A334562)
   Generating function:  exp(-(x + x^2 + x^3))
   Recurrence:  a(0) = 1,
                a(1) = a(2) = -1,
                a(n) = -a(n-1) - 2*(n-1)*a(n-2) - 3*(n-1)*(n-2)*a(n-3)   (n ≥ 3)
*/

#include <stdlib.h>   /* for malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* Memoisation table for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* allocated size of cache  */
    static idx_t last   = (idx_t)-1;  /* largest n for which cache[n] is valid */

    /* Grow cache to hold at least n+1 entries */
    if ((idx_t)cap <= n)
    {
        size_t new_cap = (size_t)(cap ? cap : 8);
        while (new_cap <= (size_t)n) new_cap <<= 1;

#ifdef __cplusplus
        seq_t *tmp = reinterpret_cast<seq_t*>(realloc(cache, new_cap * sizeof(seq_t)));
#else
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
        if (!tmp) abort();                  /* out of memory */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise base values if this is the first call */
    if (last == (idx_t)-1)
    {
        cache[0] = (seq_t)1;
        if (n >= 1) cache[1] = (seq_t)-1;
        if (n >= 2) cache[2] = (seq_t)-1;
        last = n < 2 ? n : 2;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        seq_t term1 = cache[i - 1];                            /* a(i-1)        */
        seq_t term2 = ((seq_t)(i - 1)) * cache[i - 2];          /* (i-1)*a(i-2)  */
        term2 *= (seq_t)2;                                      /* 2*(i-1)*a(i-2)*/
        seq_t term3 = ((seq_t)(i - 1)) * ((seq_t)(i - 2));      /* (i-1)(i-2)    */
        term3 *= cache[i - 3];                                  /* *a(i-3)       */
        term3 *= (seq_t)3;                                      /* 3*(i-1)(i-2)*a(i-3) */
        cache[i] = -term1 - term2 - term3;
    }

    last = n;
    return cache[n];
}
