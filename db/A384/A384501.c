/*
 * A384501  a(n) = Sum_{k=0..n} abs(Stirling1(n,k)) * Stirling2(n,n-k).
 * https://oeis.org/A384501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} abs(Stirling1(n,k)) * Stirling2(n,n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static buffers for the current and previous rows of Stirling numbers */
    static seq_t *c_prev = NULL;      /* absolute Stirling numbers of first kind */
    static seq_t *c_curr = NULL;      /* temporary buffer */
    static seq_t *s_prev = NULL;      /* Stirling numbers of second kind */
    static seq_t *s_curr = NULL;      /* temporary buffer */
    static seq_t *cache  = NULL;      /* cache of a(n) values */
    static idx_t  last_n = 0;         /* highest n computed so far */
    static idx_t  cache_size = 0;     /* number of cached values */

    /* --- initialisation for the first call --------------------------------- */
    if (c_prev == NULL) {
        /* Base case: n = 0 */
        c_prev = (seq_t *)malloc(sizeof(seq_t) * 1);
        c_prev[0] = (seq_t)1;                 /* |Stirling1(0,0)| = 1 */

        s_prev = (seq_t *)malloc(sizeof(seq_t) * 1);
        s_prev[0] = (seq_t)1;                 /* Stirling2(0,0) = 1 */

        cache  = (seq_t *)malloc(sizeof(seq_t) * 1);
        cache[0] = (seq_t)1;                  /* a(0) = 1 */

        cache_size = 1;
        last_n = 0;
    }

    /* --- extend cache and Stirling tables up to requested n ----------------- */
    if (n > last_n) {
        for (idx_t i = last_n + 1; i <= n; ++i) {
            size_t size = (size_t)(i + 1);          /* new row length */

            /* Allocate current row buffers */
            c_curr = (seq_t *)realloc(c_curr, sizeof(seq_t) * size);
            s_curr = (seq_t *)realloc(s_curr, sizeof(seq_t) * size);

            /* Compute absolute Stirling numbers of the first kind:
               |Stirling1(i,k)| = |Stirling1(i-1,k-1)| + (i-1)*|Stirling1(i-1,k)|  */
            size_t prev_len = (size_t)(last_n + 1); /* length of c_prev */
            for (idx_t k = 0; k <= i; ++k) {
                seq_t part1 = (k > 0 && (size_t)(k - 1) < prev_len) ? c_prev[k - 1] : (seq_t)0;
                seq_t part2 = (k < (idx_t)prev_len) ? c_prev[k] * (seq_t)(i - 1) : (seq_t)0;
                c_curr[k] = part1 + part2;
            }

            /* Compute Stirling numbers of the second kind:
               Stirling2(i,k) = k*Stirling2(i-1,k) + Stirling2(i-1,k-1) */
            for (idx_t k = 0; k <= i; ++k) {
                seq_t part1 = (k > 0 && k < (idx_t)prev_len) ? s_prev[k] * (seq_t)k : (seq_t)0;
                seq_t part2 = (k > 0) ? s_prev[k - 1] : (seq_t)0;
                s_curr[k] = part1 + part2;
            }

            /* Compute a(i) as the convolution of the two rows */
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k <= i; ++k) {
                sum += c_curr[i - k] * s_curr[k];
            }

            /* Store the result in the cache */
            if (i >= cache_size) {
                cache  = (seq_t *)realloc(cache, sizeof(seq_t) * (i + 1));
                cache_size = i + 1;
            }
            cache[i] = sum;

            /* Advance to the next row: swap pointers */
            seq_t *tmp;
            tmp = c_prev; c_prev = c_curr; c_curr = tmp;
            tmp = s_prev; s_prev = s_curr; s_curr = tmp;

            last_n = i;
        }
    }

    return cache[n];
}
