/*
 * A249266  Triangle read by rows: T(n,k) is the coefficient A_k in the
 *          transformation Sum_{k=0..n} x^k = Sum_{k=0..n} A_k*(x+2*(-1)^k)^k.
 * https://oeis.org/A249266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the flattened coefficient list of the triangle defined
 *            by Σ_{k=0..n} x^k = Σ_{k=0..n} A_k (x + 2*(-1)^k)^k. a(0)=1,
 *            a(1)=3, a(2)=1, a(3)=-9, …
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define MAX_M 200   /* sufficient for n up to about 10 000 */

seq_t a(idx_t n)
{
    /* Row where the nth flattened term lies */
    idx_t m = 0;
    while ((m + 1) * (m + 2) / 2 <= n)
        ++m;

    idx_t start = m * (m + 1) / 2;
    idx_t col   = n - start;
    idx_t size  = m + 1;                     /* row length */

    /* Temporary storage – static to avoid per‑call allocation */
    static seq_t bin[MAX_M][MAX_M];
    static seq_t pow2[MAX_M];
    static seq_t A[MAX_M];

    /* Binomial coefficients bin[k][j] for 0≤j≤k≤m */
    bin[0][0] = (seq_t)1;
    for (idx_t k = 1; k < size; ++k) {
        bin[k][0] = (seq_t)1;
        for (idx_t j = 1; j <= k; ++j)
            bin[k][j] = bin[k - 1][j - 1] + bin[k - 1][j];
    }

    /* Powers of 2 up to 2^m */
    pow2[0] = (seq_t)1;
    for (idx_t i = 1; i < size; ++i)
        pow2[i] = pow2[i - 1] * (seq_t)2;

    /* Solve the triangular system for A_k (k = m … 0) */
    A[m] = (seq_t)1;
    for (idx_t j = m; j-- > 0; ) {
        seq_t sum = (seq_t)0;
        for (idx_t k = j + 1; k < size; ++k) {
            seq_t term = bin[k][j] * pow2[k - j];
            if (((k * (k - j)) & 1) != 0)
                term = -term;
            sum += A[k] * term;
        }
        A[j] = (seq_t)1 - sum;
    }

    return A[col];
}
