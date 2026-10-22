/*
 * A318025  Expansion of Sum_{k>=1} (-1 + Product_{j>=1} 1/(1 - j*x^(k*j))).
 * https://oeis.org/A318025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A006906(d), where A006906(n) = sum over all
 *            partitions * of n of the product of the partition's parts
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *A_prod   = NULL;   /* A006906 values up to current maximum */
static seq_t *A_series = NULL;   /* a(n) values up to current maximum */
static idx_t  cur_max  = 0;      /* highest n that has been precomputed */

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* sequence starts at offset 1 */

    if (n > cur_max) {
        idx_t new_max = n;

        /* free old arrays if any */
        if (A_prod) {
#ifdef __cplusplus
            delete[] A_prod;
#else
            free(A_prod);
#endif
        }
        if (A_series) {
#ifdef __cplusplus
            delete[] A_series;
#else
            free(A_series);
#endif
        }

        /* allocate fresh arrays */
#ifdef __cplusplus
        A_prod   = new seq_t[new_max + 1];
        A_series = new seq_t[new_max + 1];
#else
        A_prod   = (seq_t *)malloc(sizeof(seq_t) * (new_max + 1));
        A_series = (seq_t *)malloc(sizeof(seq_t) * (new_max + 1));
#endif
        if (!A_prod || !A_series) abort();

        /* initialise DP array for A006906 */
        A_prod[0] = (seq_t)1;                /* empty partition with product 1 */
        for (idx_t i = 1; i <= new_max; ++i) A_prod[i] = (seq_t)0;

        /* compute A006906(n) = sum over partitions of n of product of parts
         * using the recurrence:
         *   dp[0] = 1;
         *   for j = 1..new_max
         *       for t = j..new_max
         *           dp[t] += j * dp[t-j];
         */
        for (idx_t j = 1; j <= new_max; ++j) {
            seq_t sj = (seq_t)j;
            for (idx_t t = j; t <= new_max; ++t) {
                A_prod[t] += sj * A_prod[t - j];
            }
        }

        /* initialise and compute a(n) via divisor summation:
         *   a[i] = sum_{d|i} A_prod[d]
         * This can be done efficiently by iterating over all divisors d
         * and adding A_prod[d] to all multiples of d.
         */
        for (idx_t i = 1; i <= new_max; ++i) A_series[i] = (seq_t)0;
        for (idx_t d = 1; d <= new_max; ++d) {
            seq_t val = A_prod[d];
            for (idx_t m = d; m <= new_max; m += d) {
                A_series[m] += val;
            }
        }

        cur_max = new_max;
    }

    return A_series[n];
}
