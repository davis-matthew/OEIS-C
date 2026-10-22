/*
 * A334162  a(0) = 1; thereafter a(n) = exp(-1/n) * Sum_{k>=0} (n*k + 1)^n /
 *          (n^k * k!).
 * https://oeis.org/A334162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * [x^n] exp(x + (exp(n*x)-1)/n) = B_n(2, n, n^2, ...,
 *            n^{n-1})
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of OEIS sequence A334162.
 * a(0) = 1; for n > 0
 * a(n) = exp(-1/n) * sum_{k>=0} (n*k + 1)^n / (n^k * k!)   (exact integer)
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Allocate arrays for Bell polynomial values B[0..n] and powers n^e. */
    size_t sz = (size_t)(n + 1);
    seq_t *B = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *pow_n = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    if (!B || !pow_n) {
        free(B);
        free(pow_n);
        return (seq_t)0;   /* allocation failed */
    }

    B[0] = (seq_t)1;          /* B_0 = 1 */

    /* Pre‑compute n^e for e = 0 .. n-1 */
    pow_n[0] = (seq_t)1;
    seq_t sn = (seq_t)n;
    for (idx_t e = 1; e < n; ++e)
        pow_n[e] = pow_n[e - 1] * sn;

    /* Compute Bell polynomials B_m for m = 1 .. n */
    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;        /* binom(m-1, 0) */
        for (idx_t k = 1; k <= m; ++k) {
            seq_t d_k = (k == 1) ? (seq_t)2 : pow_n[k - 1]; /* d_1=2, d_k=n^{k-1} for k≥2 */
            seq_t term = binom * d_k * B[m - k];
            sum += term;
            if (k < m) {
                seq_t factor = (seq_t)(m - k);
                binom = (binom * factor) / (seq_t)k;
            }
        }
        B[m] = sum;
    }

    seq_t result = B[n];
    free(B);
    free(pow_n);
    return result;
}
