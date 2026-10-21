/*
 * A275458  Expansion of g.f. hypergeom([4/9, 5/9, 7/9], [2/3, 1], 729 x).
 * https://oeis.org/A275458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A275458:  a(0)=1, for n>=1
 * a(n) = 3*(9*n-5)*(9*n-4)*(9*n-2)/(n*n*(3*n-1)) * a(n-1)
 * The recurrence produces the integer terms of the OEIS entry.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static idx_t  cache_size = 0;   /* next free index */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* ensure enough capacity */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        /* careful cast for realloc in C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t N = (seq_t)i;

        /* denominator factors: n, n, (3*n-1) */
        seq_t den[3];
        den[0] = N;
        den[1] = N;
        den[2] = 3 * N - (seq_t)1;

        /* numerator factors: 3, (9*n-5), (9*n-4), (9*n-2) */
        seq_t num[4];
        num[0] = (seq_t)3;
        num[1] = 9 * N - (seq_t)5;
        num[2] = 9 * N - (seq_t)4;
        num[3] = 9 * N - (seq_t)2;

        seq_t val = cache[i - 1];  /* previous term */

        /* multiply numerator factors one at a time,
         * cancelling denominator factors as soon as possible. */
        for (int fi = 0; fi < 4; ++fi) {
            val *= num[fi];
            for (int di = 0; di < 3; ++di) {
                if (den[di] != (seq_t)1) {
                    seq_t g = gcd(val, den[di]);
                    if (g != (seq_t)1) {
                        val /= g;
                        den[di] /= g;
                    }
                }
            }
        }

        /* any remaining denominator factors must divide val exactly. */
        for (int di = 0; di < 3; ++di) {
            if (den[di] != (seq_t)1) {
                val /= den[di];
                den[di] = (seq_t)1;
            }
        }

        cache[i] = val;
    }

    cache_size = n + 1;
    return cache[n];
}
