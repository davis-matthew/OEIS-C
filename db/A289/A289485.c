/*
 * A289485  G.f.: Product_{m>0} (1 + x^m + 2!*x^(2*m) + 3!*x^(3*m)).
 * https://oeis.org/A289485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in * Product_{m>0} (1 + x^m +
 *            2!*x^(2m) + 3!*x^(3m))
 * Range:     u8 n <= 12, u16 n <= 34, u32 n <= 103, u64 n <= 347,
 *            u128 n <= 500, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP using two buffers and iterative convolution for each part size m. */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;
    static seq_t *newdp = NULL;
    static size_t cap = 0;          /* allocated number of elements */

    /* Allocate or enlarge the buffers if needed */
    if ((size_t)n + 1 > cap) {
        if (dp)      free_seq_array(dp);
        if (newdp)   free_seq_array(newdp);
        cap = (size_t)n + 1;
        dp     = alloc_seq_array(cap);
        newdp  = alloc_seq_array(cap);
    }

    /* initialise dp[0..n] */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* Main DP over part sizes m = 1 .. n */
    for (idx_t m = 1; m <= n; ++m) {
        /* copy current dp values as the k = 0 contribution */
        for (idx_t i = 0; i <= n; ++i)
            newdp[i] = dp[i];

        /* add contributions for k = 1, 2, 3 */
        for (int k = 1; k <= 3; ++k) {
            idx_t km = k * m;
            if (km > n) break;

            seq_t weight;
            switch (k) {
                case 1: weight = (seq_t)1; break;
                case 2: weight = (seq_t)2; break;
                default: weight = (seq_t)6; /* 3! = 6 */
            }

            for (idx_t j = 0; j + km <= n; ++j)
                newdp[j + km] += dp[j] * weight;
        }

        /* swap buffers for next iteration */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    return dp[n];
}
