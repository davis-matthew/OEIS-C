/*
 * A371114  a(n) is n! times the coefficient of x in the associated Laguerre
 *          polynomial Laguerre(n,x,1).
 * https://oeis.org/A371114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 21, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A371114
   a(n) = (4*n-7)*a(n-1)
          - (6*n^2-25*n+27)*a(n-2)
          + (n-2)*(4*n^2-21*n+28)*a(n-3)
          - (n-3)^3*(n-2)*a(n-4)
   initial values: a(0)=0, a(1)=1, a(2)=1, a(3)=-1
   (vectorised version of the 4‑term recurrence given in the OEIS entry) */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;      /* array a[0..cache_len-1] */
    static size_t cache_len = 0;     /* currently allocated length */
    static size_t computed = 0;      /* number of terms actually stored */

    /* initialise the cache on the first call */
    if (cache == NULL) {
        cache_len = 8;                     /* start with a small array */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;       /* allocation failure → 0 */

        /* set the first four known terms */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)-1;
        computed = 4;
    }

    /* grow the cache if the requested index is outside the current size */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = cache_len;
        while ((size_t)n + 1 > new_len)
            new_len <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure → 0 */
        cache = tmp;
        cache_len = new_len;
    }

    /* compute any missing terms up to n */
    if (computed <= (size_t)n) {
        for (size_t i = computed; i <= (size_t)n; ++i) {
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];
            seq_t a4 = cache[i-4];
            seq_t i_seq = (seq_t)i;

            /* term1 = (4*i-7)*a1 */
            seq_t t1 = (4*i_seq - 7) * a1;

            /* term2 = -(6*i^2-25*i+27)*a2 */
            seq_t t2 = (-6*i_seq*i_seq + 25*i_seq - 27) * a2;

            /* term3 = (i-2)*(4*i^2-21*i+28)*a3 */
            seq_t t3 = (i_seq - 2) * (4*i_seq*i_seq - 21*i_seq + 28) * a3;

            /* term4 = (i-3)^3*(i-2)*a4 */
            seq_t t4 = (i_seq - 3) * (i_seq - 3) * (i_seq - 3) * (i_seq - 2) * a4;

            cache[i] = t1 + t2 + t3 - t4;
        }
        computed = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
