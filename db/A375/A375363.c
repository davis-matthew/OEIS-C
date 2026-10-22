/*
 * A375363  Expansion of 1/( (1 + x)^2 * (1 - x*(1 + x)^3) ).
 * https://oeis.org/A375363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -a(n-1) + 4*a(n-2) + 10*a(n-3) + 10*a(n-4) + 5*a(n-5) +
 *            a(n-6) with initial values a(0)=1, a(1)=-1, a(2)=5, a(3)=1,
 *            a(4)=19, a(5)=30
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 23, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* computed values */
    static idx_t sz = 0;           /* size of cache allocated */

    if (n >= sz) {
        /* grow cache to accommodate n */
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();          /* memory exhaustion */
        cache = tmp;

        /* compute missing terms */
        for (idx_t i = sz; i <= n; ++i) {
            if (i == 0)            cache[0] = 1;
            else if (i == 1)       cache[1] = -1;
            else if (i == 2)       cache[2] = 5;
            else if (i == 3)       cache[3] = 1;
            else if (i == 4)       cache[4] = 19;
            else if (i == 5)       cache[5] = 30;
            else
                cache[i] = -cache[i-1]
                          + ((seq_t)4) * cache[i-2]
                          + ((seq_t)10) * cache[i-3]
                          + ((seq_t)10) * cache[i-4]
                          + ((seq_t)5)  * cache[i-5]
                          + cache[i-6];
        }
        sz = n + 1;
    }
    return cache[n];
}
