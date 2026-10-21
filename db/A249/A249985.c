/*
 * A249985  Number of length 5+1 0..2*n arrays with the sum of the absolute
 *          values of adjacent differences equal to 5*n.
 * https://oeis.org/A249985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 55, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical linear recurrence for A249985
   a(n) = 2*a(n-1) + 3*a(n-2) - 8*a(n-3) - 2*a(n-4) + 12*a(n-5)
          - 2*a(n-6) - 8*a(n-7) + 3*a(n-8) + 2*a(n-9) - a(n-10)
   Initial terms:
     a(1)=136, a(2)=1606, a(3)=6856, a(4)=22560,
     a(5)=55372, a(6)=123154, a(7)=237348,
     a(8)=434042, a(9)=728724, a(10)=1184066
*/

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of already computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored (a(1) at cache[0]) */
    static size_t cache_cap = 0;   /* current capacity */

    if (n < 1) return 0;          /* undefined for n < 1 */

    /* Ensure we have computed up to n */
    if (cache_len < (size_t)n) {
        size_t need = (size_t)n;
        if (need < 10) need = 10;                 /* at least initial 10 terms */

        /* grow buffer if necessary */
        if (need > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap : 16;
            while (new_cap < need) new_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* static initial values */
        static const seq_t init[10] = {
            136, 1606, 6856, 22560, 55372,
            123154, 237348, 434042, 728724, 1184066
        };

        /* compute missing terms */
        size_t i;
        for (i = cache_len; i < need; ++i) {
            if (i < 10) {
                cache[i] = init[i];
            } else {
                /* indices: j = i (0‑based), corresponds to a(j+1) */
                size_t j = i;

                /* positive contributions */
                seq_t pos = (seq_t)2 * cache[j-1]  /* a(n-1) */
                           + (seq_t)3 * cache[j-2]  /* a(n-2) */
                           + (seq_t)12 * cache[j-5] /* a(n-5) */
                           + (seq_t)3 * cache[j-8]  /* a(n-8) */
                           + (seq_t)2 * cache[j-9]; /* a(n-9) */

                /* negative contributions */
                seq_t neg = (seq_t)8 * cache[j-3]  /* a(n-3) */
                           + (seq_t)2 * cache[j-4]  /* a(n-4) */
                           + (seq_t)2 * cache[j-6]  /* a(n-6) */
                           + (seq_t)8 * cache[j-7]  /* a(n-7) */
                           + cache[j-10];          /* a(n-10) */

                cache[j] = pos - neg;
            }
        }
        cache_len = need;
    }

    return cache[(size_t)n - 1];
}
