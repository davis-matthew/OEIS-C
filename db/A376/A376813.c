/*
 * A376813  G.f.: Sum_{k>=0} x^(k*(k+1)) * Product_{j=1..k} (1 + x^j)^2.
 * https://oeis.org/A376813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in Sum_{k>=0} x^{k*(k+1)} *
 *            Prod_{j=1..k} (1 + x^j)^2
 * Range:     u8 n <= 62, u16 n <= 176, u32 n <= 548, u64 n <= 1874,
 *            u128 n <= 6812, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Special case for n == 0 */
    if (n == 0) return (seq_t)1;

    /* Find k_max: largest k such that k*(k+1) <= n */
    double d = sqrt((double)1 + 4.0 * (double)n);
    idx_t k_max = (idx_t)((-1.0 + d) * 0.5);
    while ((k_max + 1) * (k_max + 2) <= n) ++k_max;
    while (k_max * (k_max + 1) > n) --k_max;

    /* Maximum subset sum needed: T_kmax = k_max*(k_max+1)/2 */
    idx_t max_sum = (k_max * (k_max + 1)) / 2;

    /* Allocate array for subset-sum counts D[j] */
    seq_t *D = (seq_t *)calloc(max_sum + 1, sizeof(seq_t));
    if (!D) return (seq_t)0;   /* Memory failure: return 0 (should not happen) */
    D[0] = (seq_t)1;           /* Empty subset */

    idx_t cur_max_sum = 0;
    seq_t result = (seq_t)0;

    /* Enumerate k from 1 to k_max, updating D each time and adding contributions */
    for (idx_t k = 1; k <= k_max; ++k) {
        /* Update D to include the new element k */
        idx_t new_max = cur_max_sum + k;   /* equals k*(k+1)/2 */
        for (idx_t j = new_max; j >= k; --j) {
            D[j] += D[j - k];
        }
        cur_max_sum = new_max;
        idx_t T_k = cur_max_sum;                /* k(k+1)/2 */
        idx_t kkp1 = k * (k + 1);                /* k(k+1) */
        idx_t m = n - kkp1;                      /* n - k(k+1) */

        /* If m is outside the possible range for this k, skip */
        if (m > (idx_t)2 * T_k) continue;

        /* Sum over j to compute convolution for this m */
        idx_t start = (m > T_k) ? m - T_k : 0;
        idx_t end   = (m < T_k) ? m : T_k;
        for (idx_t j = start; j <= end; ++j) {
            result += D[j] * D[m - j];
        }
    }

    free(D);
    return result;
}
