/*
 * A289916  Coefficients of 1/(Sum_{k>=0} round((k+1)*r)(-x)^k), where r =
 *          9/7.
 * https://oeis.org/A289916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 40, u64 n <= 80, u128 n <= 162,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A289916
   a(0)=1, a(1)=3, a(2)=5, a(3)=8, a(4)=13, a(5)=22
   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4) + 2*a(n-5) - a(n-6)
   + (n==7 || n==8 ? 1 : 0)     for all n>5
*/
#include <stdlib.h>

static seq_t *cache = NULL;    /* array holding computed terms      */
static idx_t  cache_len  = 0;  /* allocated array length             */
static idx_t  max_computed = 0;/* highest index that has been computed */

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL) {
        /* at least space for indices 0..8 to handle the +1 adjustments    */
        cache_len = 9;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)8;
        cache[4] = (seq_t)13;
        cache[5] = (seq_t)22;
        max_computed = 5;
    }

    /* expand array and compute missing values up to n */
    if (n > max_computed) {
        if (n + 1 > cache_len) {
            /* grow the array to at least n+1 elements */
            cache_len = n + 1;
            seq_t *tmp = (seq_t*)realloc(cache, cache_len * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
        }

        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t val;
            if (i >= 6) {
                /* recurrence part */
                val = 2 * cache[i-1]
                    - cache[i-2]
                    + cache[i-3]
                    - cache[i-4]
                    + 2 * cache[i-5]
                    - cache[i-6];
                /* special inhomogeneous terms for n==7 and n==8 */
                if (i == 7 || i == 8) {
                    val += (seq_t)1;
                }
            } else {
                /* should not happen because indices <6 are already set */
                val = cache[i];
            }
            cache[i] = val;
        }
        max_computed = n;
    }

    return cache[n];
}
