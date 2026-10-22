/*
 * A334242  a(n) = exp(-n) * Sum_{k>=0} (k + n)^n * n^k / k!.
 * https://oeis.org/A334242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exp(-n) * Sum_{k>=0} (k + n)^n * n^k / k!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the identity  
   a(n) = sum_{k=0}^n binom(n,k) * n^k * B_{n-k}(n)   where
   B_m(t) = t * sum_{i=0}^{m-1} binom(m-1,i) * B_i(t)  (combinatorial Bell polynomial) */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Allocate array for Bell polynomials B[0..n] */
    seq_t *B = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!B) return (seq_t)0;   /* allocation failure */

    /* B[0] = 1 */
    B[0] = (seq_t)1;

    /* Compute B[1..n] for parameter t = n */
    for (idx_t k = 0; k < n; ++k) {
        seq_t binom = (seq_t)1;      /* binom(k,i) for i = 0 */
        seq_t sum_s = (seq_t)0;
        for (idx_t i = 0; i <= k; ++i) {
            sum_s += binom * B[i];
            if (i < k) {
                binom = binom * (seq_t)(k - i) / (seq_t)(i + 1);
            }
        }
        B[k + 1] = (seq_t)n * sum_s;
    }

    /* Now compute a(n) = sum_{k=0}^n binom(n,k) * n^k * B_{n-k} */
    seq_t sum = (seq_t)0;
    seq_t binom_j = (seq_t)1;               /* binom(n,0) */
    seq_t pow_current = (seq_t)1;           /* n^0 */

    for (idx_t j = 0; j <= n; ++j) {
        seq_t term = binom_j * B[n - j];
        term *= pow_current;
        sum += term;

        if (j < n) {
            binom_j = binom_j * (seq_t)(n - j) / (seq_t)(j + 1);
        }
        if (j < n) {
            pow_current *= (seq_t)n;
        }
    }

    free(B);
    return sum;
}
