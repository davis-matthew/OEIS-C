/*
 * A375061  Expansion of 1 / Sum_{k in Z} x^(2*k) / (1 - x^(5*k+2)).
 * https://oeis.org/A375061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in the power series expansion of * 1 /
 *            ( Sum_{k in Z} x^(2k) / (1 - x^(5k+2)) ) * Computed via series
 *            inversion: Let S(x) = Sum_{k in Z} x^(2k) / (1 - x^(5k+2)) *
 *            Then a(0)=1 and for n≥1: * a(n) = - Σ_{k=1..n} S_k * a(n-k) *
 *            where S_k are coefficients of S(x). * S_k are...
 * Range:     i8 n <= 17, i16 n <= 49, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    /* Allocate coefficient arrays */
    seq_t *S = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *A = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!S || !A) { abort(); }

    /* Initialize to zero */
    for (size_t i = 0; i <= N; ++i) {
        S[i] = 0;
        A[i] = 0;
    }

    /* Generate S(x) coefficients:
       Positive part: k >= 0
         exponent = 2*k + m*(5*k+2), add +1.
       Negative part (k' = -k, k' <= -1):
         exponent = (3*k-2) + m*(5*k-2), add -1.   (k >= 1)
    */
    /* Positive part */
    for (size_t k = 0; ; ++k) {
        size_t start = 2 * k;
        if (start > N) break;
        size_t step = 5 * k + 2;
        if (step == 0) break;          /* safety, shouldn't happen */
        for (size_t e = start; e <= N; e += step) {
            S[e] += 1;
        }
    }

    /* Negative part */
    for (size_t k = 1; ; ++k) {
        size_t start = 3 * k - 2;
        if (start > N) break;
        size_t step = 5 * k - 2;
        if (step == 0) break;          /* safety, shouldn't happen */
        for (size_t e = start; e <= N; e += step) {
            S[e] += -1;
        }
    }

    /* Series inversion: A(x) = 1 / S(x) */
    A[0] = 1;           /* S_0 = 1 by construction */
    for (size_t npos = 1; npos <= N; ++npos) {
        seq_t sum = 0;
        for (size_t k = 1; k <= npos; ++k) {
            sum += S[k] * A[npos - k];
        }
        A[npos] = -sum;
    }

    seq_t result = A[N];
    free(S);
    free(A);
    return result;
}
