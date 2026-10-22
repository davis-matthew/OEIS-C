/*
 * A349453  Dirichlet inverse of A133494, 3^(n-1).
 * https://oeis.org/A349453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1)=1; a(n)= -∑_{d|n, d<n} 3^(n/d-1) * a(d) (Dirichlet inverse
 *            of 3^(n-1))
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 20, i64 n <= 40, i128 n <= 81,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* cache[1..cap] holds a[1]..a[cap] */
    static size_t cap = 0;

    if (n == 0) return 0;          /* undefined, but harmless */

    if ((idx_t)n > cap)            /* need to extend the cache */
    {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();        /* memory exhausted */
        cache = tmp;

        for (idx_t idx = (idx_t)(cap + 1); idx <= n; ++idx)
        {
            if (idx == 1) {
                cache[1] = 1;
                continue;
            }

            seq_t sum = 0;
            idx_t i = idx;

            for (idx_t d = 1; d <= i / d; ++d)
            {
                if (i % d == 0)
                {
                    /* divisor d1 = d */
                    if (d < i)
                    {
                        idx_t div1 = i / d;
                        seq_t pow1 = pow3(div1 - 1);
                        sum += pow1 * cache[d];
                    }

                    /* divisor d2 = i/d, avoid double counting */
                    idx_t d2 = i / d;
                    if (d2 != d && d2 < i)
                    {
                        idx_t div2 = i / d2;
                        seq_t pow2 = pow3(div2 - 1);
                        sum += pow2 * cache[d2];
                    }
                }
            }

            cache[idx] = -sum;
        }
        cap = (size_t)n;
    }

    return cache[n];
}
