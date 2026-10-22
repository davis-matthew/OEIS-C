/*
 * A387872  a(n) = Sum_{k=0..n} binomial(3*n+1,5*k).
 * https://oeis.org/A387872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 5*a(n-2) + 145*a(n-3) + 55*a(n-4) + 8*a(n-5)
 *            (a(0)=1, a(1)=1, a(2)=22, a(3)=254, a(4)=1574)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached values */
    static size_t cap   = 0;      /* allocated capacity */
    static idx_t  len   = 0;      /* number of terms stored */

    /* First call: allocate enough space for at least the first 5 terms */
    if (cache == NULL) {
        cap = (size_t)((n + 1) > 5 ? (n + 1) : 5);
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (!cache)
            exit(EXIT_FAILURE);

        /* initialise new slots to zero */
        for (size_t i = 0; i < cap; ++i)
            cache[i] = (seq_t)0;

        /* compute the first five terms via the binomial definition */
        for (idx_t i = 0; i <= 4; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k <= i; ++k)
                sum += binom(3 * i + 1, 5 * k);
            cache[i] = sum;
        }
        len = 5;                      /* we have indices 0..4 ready */
    }

    /* Expand cache if the requested index is beyond current capacity */
    if ((size_t)(n + 1) > cap) {
        size_t old_cap = cap;
        while (cap < (size_t)(n + 1))
            cap <<= 1;                /* double */
        seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);
        cache = tmp;
        for (size_t i = old_cap; i < cap; ++i)
            cache[i] = (seq_t)0;
    }

    /* Compute any missing terms using the recurrence */
    while (len <= n) {
        cache[len] =  5 * cache[len - 1]
                    + 5 * cache[len - 2]
                    + 145 * cache[len - 3]
                    + 55 * cache[len - 4]
                    + 8 * cache[len - 5];
        ++len;
    }

    return cache[n];
}
