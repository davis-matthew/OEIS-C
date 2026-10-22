/*
 * A369755  Expansion of e.g.f. exp( (1 - (1+x)^4)/4 ).
 * https://oeis.org/A369755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 22, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A369755: The sequence defined by the D‑finite recurrence  
 * a(0)=1, a(1)=-1, a(2)=-2, a(3)=2, 
 * a(n)+a(n-1)+3*(n-1)*a(n-2)+3*(n-1)*(n-2)*a(n-3)+(n-1)*(n-2)*(n-3)*a(n-4)=0
 *
 * This implementation memoises values in a growing static array, computes
 * each new term with the recurrence and returns the requested value.
 * It works for any seq_t type (fixed width or arbitrary precision) and
 * any idx_t type (here uint64_t).  No external utilities are required.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* ensure we have space for indices up to n */
    size_t req = (size_t)n + 1;
    if (req > cache_size)
    {
        size_t old_size = cache_size;
        if (cache_size == 0)
        {
            cache = (seq_t *)malloc(req * sizeof(seq_t));
            cache_size = req;
        }
        else
        {
            seq_t *tmp = (seq_t *)realloc(cache, req * sizeof(seq_t));
            if (tmp == NULL) return 0; /* allocation failure – handled by caller */
            cache = tmp;
            cache_size = req;
        }

        /* compute new entries */
        for (size_t i = old_size; i < req; ++i)
        {
            if (i == 0)
                cache[0] = (seq_t)1;
            else if (i == 1)
                cache[1] = (seq_t)-1;
            else if (i == 2)
                cache[2] = (seq_t)-2;
            else if (i == 3)
                cache[3] = (seq_t)2;
            else
            {
                const seq_t a1 = cache[i - 1];
                const seq_t a2 = cache[i - 2];
                const seq_t a3 = cache[i - 3];
                const seq_t a4 = cache[i - 4];

                const seq_t s1 = a1;                                       /* a(n-1) */
                const seq_t s2 = (seq_t)(i - 1) * ((seq_t)3 * a2);          /* 3*(n-1)*a(n-2) */
                const seq_t s3 = ((seq_t)(i - 1) * (seq_t)(i - 2)) * ((seq_t)3 * a3);
                /* 3*(n-1)*(n-2)*a(n-3) */
                const seq_t s4 = ((seq_t)(i - 1) * (seq_t)(i - 2)) *
                                 (seq_t)(i - 3) * a4; /* (n-1)*(n-2)*(n-3)*a(n-4) */

                cache[i] = -(s1 + s2 + s3 + s4);
            }
        }
    }

    return cache[(size_t)n];
}
