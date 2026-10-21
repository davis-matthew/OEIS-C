/*
 * A276280  Number of triangular partitions of n of order 9.
 * https://oeis.org/A276280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in
 *            1/((1-x)^9*(1-x^3)^8*(1-x^5)^7*(1-x^7)^6*(1-x^9)^5*(1-x^11)^4*(1-x^13)^3*(1-x^15)^2*(1-x^17))
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a dynamic programming convolution over the
   factors (1-x^k)^(-m), where m is the exponent for part k.  The
   convolution is performed in O(n * sum( n / k )) time. */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* static cache for results up to the largest n seen so far          */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;   /* number of elements in dp array       */

    if (n >= dp_size) {                 /* need to (re)compute up to n      */
        /* Free old cache if it exists */
        free(dp);
        dp_size = n + 1;
        dp = (seq_t *)malloc((size_t)dp_size * sizeof(seq_t));
        if (!dp) return 0;              /* allocation failure, unlikely  */

        /* Initialize DP array */
        for (idx_t i = 0; i < dp_size; ++i)
            dp[i] = 0;
        dp[0] = 1;

        /* The set of parts and corresponding exponents from the generator */
        const idx_t ks[] = {1,3,5,7,9,11,13,15,17};
        const idx_t ms[] = {9,8,7,6,5,4,3,2,1};
        const int num_k = sizeof(ks)/sizeof(ks[0]);

        for (int ki = 0; ki < num_k; ++ki) {
            idx_t k = ks[ki];
            idx_t m = ms[ki];

            /* Pre‑compute binomial coefficients for this part up to needed t */
            idx_t max_t = n / k;                               /* max usage count */
            seq_t *w = (seq_t *)malloc((size_t)(max_t + 1) * sizeof(seq_t));
            if (!w) return 0;                                 /* unlikely */
            for (idx_t t = 0; t <= max_t; ++t) {
                w[t] = binomial(t + m - 1, m - 1);
            }

            /* Allocate array for the convolution result */
            seq_t *newdp = (seq_t *)malloc((size_t)dp_size * sizeof(seq_t));
            if (!newdp) return 0;                             /* unlikely */
            for (idx_t i = 0; i < dp_size; ++i)
                newdp[i] = 0;

            /* Perform the convolution */
            for (idx_t i = 0; i <= n; ++i) {
                seq_t dpi = dp[i];
                if (dpi == 0) continue;

                /* The largest t that can still fit into n when adding i */
                idx_t max_t_local = (n - i) / k;
                for (idx_t t = 0; t <= max_t_local; ++t) {
                    idx_t idx = i + t * k;
                    newdp[idx] += dpi * w[t];
                }
            }

            /* Clean up and set up for next stage */
            free(w);
            free(dp);
            dp = newdp;
        }
    }

    return dp[n];
}
