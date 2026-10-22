/*
 * A356600  a(n) = n! * Sum_{k=1..n} sigma_2(k)/(k * (n-k)!).
 * https://oeis.org/A356600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} sigma_2(k)/(k * (n-k)!)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for n = 0 */

    /* allocate arrays of size (n+1) */
    const size_t sz = (size_t)n + 1;
    seq_t *sigma2 = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *fact   = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *A      = (seq_t*)calloc(sz, sizeof(seq_t));

    /* sigma_2(k) = sum_{d|k} d^2 */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sq = (seq_t)i * (seq_t)i;
        for (idx_t j = i; j <= n; j += i)
            sigma2[j] += sq;
    }

    /* factorials */
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact[i] = fact[i-1] * (seq_t)i;

    /* A(k) = sigma_2(k) * (k-1)! */
    for (idx_t k = 1; k <= n; ++k)
        A[k] = sigma2[k] * fact[k-1];

    /* compute the weighted sum using the binomial coefficients */
    seq_t sum   = (seq_t)0;
    seq_t coeff = (seq_t)1;                 /* C(n,0) */
    for (idx_t k = 1; k <= n; ++k) {
        coeff = coeff * (seq_t)(n - k + 1) / (seq_t)k;   /* C(n,k) */
        sum   += coeff * A[k];
    }

    free(sigma2);
    free(fact);
    free(A);
    return sum;
}
