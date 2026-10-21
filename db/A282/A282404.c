/*
 * A282404  Coefficients in q-expansion of E_4*E_6^4, where E_4 and E_6 are
 *          respectively the Eisenstein series A004009 and A013973.
 * https://oeis.org/A282404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of (1 + 240 Σ σ₃(k) q^k) * (1 - 504 Σ σ₅(k)
 *            q^k)^4 , where σ₃ and σ₅ are the divisor sum functions of
 *            exponent 3 and 5
 * Range:     i8 n <= 0, i16 n <= 1, i32 n <= 3, i64 n <= 8, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm:
   1. Compute σ₃[0..n] and σ₅[0..n] using divisor sieving.
   2. Let h[k] = -504 σ₅[k]  (h[0] = 0).
   3. Compute h², h³, h⁴ by convolution of the series.
   4. e6four[k] = 1 + 4h[k] + 6h²[k] + 4h³[k] + h⁴[k] (k ≥ 0).
   5. a[n] = e6four[n] + 240 Σ_{i=1}^{n} σ₃[i] * e6four[n-i].   */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    /* Allocate arrays of length n+1 (indices 0 .. n). */
    size_t sz = (size_t)(n + 1);
    seq_t *sigma3 = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *sigma5 = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *h      = (seq_t*)calloc(sz, sizeof(seq_t));      /* -504*σ₅ */
    seq_t *h2     = (seq_t*)calloc(sz, sizeof(seq_t));      /* h*h      */
    seq_t *h3     = (seq_t*)calloc(sz, sizeof(seq_t));      /* h*h²     */
    seq_t *h4     = (seq_t*)calloc(sz, sizeof(seq_t));      /* h²*h²    */
    seq_t *e6four = (seq_t*)calloc(sz, sizeof(seq_t));      /* E_6^4    */
    seq_t *a_val  = (seq_t*)calloc(sz, sizeof(seq_t));      /* final seq*/

    /* ------------------------------------------------------------------ */
    /* 1. Divisor sums σ₃ and σ₅ */
    for (idx_t d = 1; d <= n; ++d)
    {
        seq_t dseq = (seq_t)d;
        seq_t d2   = dseq * dseq;
        seq_t d3   = d2 * dseq;
        seq_t d5   = d3 * d2;          /* d^5 */

        for (idx_t m = d; m <= n; m += d)
        {
            sigma3[m] += d3;
            sigma5[m] += d5;
        }
    }

    /* ------------------------------------------------------------------ */
    /* 2. h[k] = -504 * σ₅[k] (h[0] stays 0) */
    const seq_t minus504 = -(seq_t)504;
    for (idx_t i = 1; i <= n; ++i)
        h[i] = minus504 * sigma5[i];

    /* ------------------------------------------------------------------ */
    /* 3. Convolutions to obtain h², h³, h⁴ */
    /* h²[k] = Σ_{i=1}^{k-1} h[i]*h[k-i]   (k ≥ 1) */
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t sum = (seq_t)0;
        for (idx_t i = 1; i < k; ++i)
            sum += h[i] * h[k - i];
        h2[k] = sum;
    }

    /* h³[k] = Σ_{i=1}^{k-1} h²[i]*h[k-i] */
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t sum = (seq_t)0;
        for (idx_t i = 1; i < k; ++i)
            sum += h2[i] * h[k - i];
        h3[k] = sum;
    }

    /* h⁴[k] = Σ_{i=1}^{k-1} h²[i]*h²[k-i] */
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t sum = (seq_t)0;
        for (idx_t i = 1; i < k; ++i)
            sum += h2[i] * h2[k - i];
        h4[k] = sum;
    }

    /* ------------------------------------------------------------------ */
    /* 4. Compute e6four (coefficients of E_6^4) */
    e6four[0] = (seq_t)1;          /* constant term */
    for (idx_t k = 1; k <= n; ++k)
    {
        e6four[k] = 4 * h[k] + 6 * h2[k] + 4 * h3[k] + h4[k];
    }

    /* ------------------------------------------------------------------ */
    /* 5. Compute a[n] = coefficient of E_4 * E_6^4 */
    a_val[0] = (seq_t)1;           /* constant term */
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t sum = (seq_t)0;
        for (idx_t i = 1; i <= k; ++i)
            sum += sigma3[i] * e6four[k - i];
        a_val[k] = e6four[k] + (seq_t)240 * sum;
    }

    seq_t result = a_val[n];

    /* ------------------------------------------------------------------ */
    /* Clean up */
    free(sigma3);
    free(sigma5);
    free(h);
    free(h2);
    free(h3);
    free(h4);
    free(e6four);
    free(a_val);

    return result;
}
