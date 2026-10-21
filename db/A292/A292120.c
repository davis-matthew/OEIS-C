/*
 * A292120  E.g.f.: A(x) + B(x) + C(x) where A'(x) = B(x)*C(x), B'(x) =
 *          A(x)*C(x), and C'(x) = A(x)*B(x), where A(x), B(x), and C(x) are
 *          the e.g.f.s of A292121, A292122, and A292123, respectively.
 * https://oeis.org/A292120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A292120:  a(n) = sum_{k=0}^{n} A_k + B_k + C_k, where
 *   A[0] = 1, B[0] = 2, C[0] = 3
 *   For each k >= 0:
 *     A[k+1] = Σ_{i=0}^{k} binom(k,i) * B[i] * C[k-i]
 *     B[k+1] = Σ_{i=0}^{k} binom(k,i) * A[i] * C[k-i]
 *     C[k+1] = Σ_{i=0}^{k} binom(k,i) * A[i] * B[k-i]
 *
 * The above follows from A',B',C' = B*C, A*C, A*B and the
 * exponential generating function multiplication rules.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef __cplusplus
/* C compiler */
static seq_t *seq_alloc(size_t n) { return (seq_t*)calloc(n, sizeof(seq_t)); }
#else
/* C++ compiler */
static seq_t *seq_alloc(size_t n) { return new seq_t[n](); }
#endif

seq_t a(idx_t n)
{
    /* Allocate arrays of size n+1 */
    size_t N = (size_t)n + 1;
    seq_t *A = seq_alloc(N);
    seq_t *B = seq_alloc(N);
    seq_t *C = seq_alloc(N);
    if (!A || !B || !C) return (seq_t)0; /* allocation failure fallback */

    /* Initial values */
    A[0] = (seq_t)1;
    B[0] = (seq_t)2;
    C[0] = (seq_t)3;

    /* Main recurrence loop */
    for (idx_t k = 0; k < n; ++k) {
        seq_t bin = (seq_t)1;    /* binom(k,0) */
        seq_t sumBC = (seq_t)0;  /* for A[k+1] */
        seq_t sumAC = (seq_t)0;  /* for B[k+1] */
        seq_t sumAB = (seq_t)0;  /* for C[k+1] */

        for (idx_t i = 0; i <= k; ++i) {
            /* accumulate weighted products */
            sumBC += bin * B[i] * C[k - i];
            sumAC += bin * A[i] * C[k - i];
            sumAB += bin * A[i] * B[k - i];

            /* update binom(k,i+1) for next iteration */
            if (i < k) {
                seq_t tmp = bin * (seq_t)(k - i);
                tmp /= (seq_t)(i + 1);       /* exact division */
                bin = tmp;
            }
        }

        A[k + 1] = sumBC;
        B[k + 1] = sumAC;
        C[k + 1] = sumAB;
    }

    seq_t result = A[n] + B[n] + C[n];

    /* Free temporary storage */
#ifndef __cplusplus
    free(A);
    free(B);
    free(C);
#else
    delete[] A;
    delete[] B;
    delete[] C;
#endif

    return result;
}
