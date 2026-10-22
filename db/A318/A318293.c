/*
 * A318293  E.g.f. satisfies y'' + y' + x^3*y = 0 with y(0)=0, y'(0)=1.
 * https://oeis.org/A318293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   (n+3)(n+2)(n+1)a(n) + a(n+4) + a(n+5) = 0 Initial values:
 *            a(0)=0, a(1)=1, a(2)=-1, a(3)=1, a(4)=-1, a(5)=1
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 18, i64 n <= 30, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for the sequence, indices 0..cap-1 */
    static seq_t *cache   = NULL;   /* array of computed a[i] */
    static size_t  cap    = 0;      /* current capacity of cache     */
    static idx_t   last   = 0;      /* highest index that has been computed */

    /* Ensure cache large enough for requested index n */
    if (cap <= (size_t)n)
    {
        size_t new_cap = (cap < 6) ? 6 : cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the five known terms on first call */
    if (last == 0)
    {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)-1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)-1;
        cache[5] = (seq_t)1;
        last    = 5;               /* highest index we have at this point */
    }

    /* Compute forward using the recurrence up to index n */
    if (n > last)
    {
        /* start from the first index whose a[i+5] might not have been
           computed yet; for safety we start at max(0, last-5) */
        idx_t start = (last >= 5) ? last - 5 : 0;

        for (idx_t i = start; i <= n - 5; ++i)
        {
            seq_t tri  = (seq_t)(i + 3) * (seq_t)(i + 2) * (seq_t)(i + 1);
            cache[i + 5] = - tri * cache[i] - cache[i + 4];
        }
        last = n;
    }

    return cache[n];
}
