/*
 * A397283  Expansion of e.g.f. (1/x) * Series_Reversion( x + log(1-x^4) ).
 * https://oeis.org/A397283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=ceil(n/4)..floor(n/3)} (4*k)!/k! *
 *            |Stirling1(k,4*k-n)|
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 11, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Determine the range of k in the sum */
    idx_t k_min = (n + 3) / 4;   /* ceil(n/4) */
    idx_t k_max = n / 3;         /* floor(n/3) */

    seq_t sum = 0;

    /* Handle the k = 0 term separately (only when n = 0) */
    if (k_min == 0) {
        /* Stirling1(0,0) = 1 and (0)!/0! = 1 */
        sum = 1;
    }

    /* For n = 0, k_max == 0, we can return immediately */
    if (k_max == 0) {
        return sum / (n + 1);
    }

    /* Allocate temporary arrays for Stirling numbers */
    size_t sz = (size_t)k_max + 1;
    seq_t *prev = (seq_t*)malloc(sz * sizeof(seq_t));
    seq_t *curr = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!prev || !curr) {
        /* Allocation failed – return 0 as a safe fallback */
        if (prev) free(prev);
        if (curr) free(curr);
        return 0;
    }

    /* Initialize the first row (S(0,0) = 1) */
    prev[0] = 1;

    /* Keep track of (4k)!/k! iteratively */
    seq_t ratio = 1;        /* for k = 0 */

    /* Iterate over k = 1 .. k_max, building Stirling rows on the fly */
    for (idx_t k = 1; k <= k_max; ++k) {
        curr[0] = 0;

        /* Compute the k-th row of unsigned Stirling numbers of the first kind */
        for (idx_t j = 1; j <= k; ++j) {
            seq_t term1 = prev[j - 1];
            seq_t term2 = 0;
            if (j <= k - 1) {
                term2 = prev[j] * (k - 1);
            }
            curr[j] = term1 + term2;
        }

        /* Update factorial ratio (4k)!/k! iteratively:
           ratio_new = ratio_old * (4k)*(4k-1)*(4k-2)*(4k-3) / k
        */
        seq_t prod = (seq_t)(4 * k) * (4 * k - 1) * (4 * k - 2) * (4 * k - 3);
        ratio = (ratio * prod) / k;

        /* If this k is in the required summation range, add its contribution */
        if (k >= k_min) {
            idx_t m = 4 * k - n;   /* 0 <= m <= k by construction */
            seq_t val_s = curr[m];
            seq_t term = val_s * ratio;
            sum += term;
        }

        /* Prepare for the next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    free(prev);
    free(curr);

    /* Final division by (n+1) – the result is guaranteed to be exact */
    return sum / (n + 1);
}
