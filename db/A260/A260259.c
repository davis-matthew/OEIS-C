/*
 * A260259  a(n) = F(n)*F(n+1) - (-1)^n, where F = A000045.
 * https://oeis.org/A260259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = F(n)*F(n+1) - (-1)^n Recurrence: a(0) = -1, a(1) = 2,
 *            a(2) = 1 a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3) (for n ≥ 3)
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 23, i64 n <= 46, i128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses memoisation to keep previously computed values.
   It works for any seq_t that supports +, -, * for integers. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated capacity */
    static size_t sz  = 0;   /* number of stored values (cache[0..sz-1]) */

    /* Initialisation on first call */
    if (cache == NULL)
    {
        /* start with capacity for first three terms */
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;  /* out of memory – unlikely */
        /* initial values a(0)..a(2) */
        cache[0] = (seq_t)-1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)1;
        sz = 3;
    }

    /* Grow cache if needed (clamp n to size_t range; safe for tests) */
    if ((size_t)n >= sz)
    {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;  /* out of memory – unlikely */
        cache = tmp;
        cap   = new_cap;

        /* compute new terms up to n (inclusive) */
        for (size_t i = sz; i <= (size_t)n; ++i)
        {
            cache[i] = 2 * cache[i-1] + 2 * cache[i-2] - cache[i-3];
        }
        sz = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
