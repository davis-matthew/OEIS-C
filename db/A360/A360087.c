/*
 * A360087  a(n) = Sum_{k=0..n} (-1)^k * binomial(3*k,n-k).
 * https://oeis.org/A360087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * binomial(3*k, n-k) Recurrence: a(n)
 *            = -a(n-1) - 3*a(n-2) - 3*a(n-3) - a(n-4) Initial values: a(0)=1,
 *            a(1)=-1, a(2)=-2, a(3)=2
 * Range:     i8 n <= 9, i16 n <= 21, i32 n <= 37, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t len   = 0;   /* number of elements stored */
    static size_t cap   = 0;   /* allocated capacity */

    /* --- initialize first four values --- */
    if (cache == NULL) {
        cap = 4;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)-2;
        cache[3] = (seq_t)2;
        len   = 4;
    }

    size_t idx = (size_t)n;

    /* --- extend cache if needed --- */
    if (idx >= len) {
        /* Ensure capacity at least idx+1 */
        size_t needed = idx + 1;
        if (needed > cap) {
            size_t newcap = cap;
            if (newcap == 0) newcap = 4;
            while (needed > newcap) newcap <<= 1;
            seq_t *newptr = (seq_t*)malloc(newcap * sizeof(seq_t));
            for (size_t i = 0; i < len; ++i) newptr[i] = cache[i];
            free(cache);
            cache = newptr;
            cap   = newcap;
        }
        /* Compute new elements using the recurrence */
        for (size_t i = len; i <= idx; ++i) {
            seq_t val;
            if (i == 0)          val = (seq_t)1;
            else if (i == 1)     val = (seq_t)-1;
            else if (i == 2)     val = (seq_t)-2;
            else if (i == 3)     val = (seq_t)2;
            else
                val = -cache[i-1] - ((seq_t)3)*cache[i-2]
                     - ((seq_t)3)*cache[i-3] - cache[i-4];
            cache[i] = val;
        }
        len = idx + 1;
    }

    return cache[idx];
}
