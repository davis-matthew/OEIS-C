/*
 * A365528  a(n) = Sum_{k=0..floor(n/5)} Stirling2(n,5*k).
 * https://oeis.org/A365528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Sum_{k=0..floor(n/5)} Stirling2(n,5*k)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: let A(0)=1,B(0)=C(0)=D(0)=E(0)=0; for m≥0
 * B(m+1)=∑_{k=0..m} binom(m,k)*A(k)
 * C(m+1)=∑_{k=0..m} binom(m,k)*B(k)
 * D(m+1)=∑_{k=0..m} binom(m,k)*C(k)
 * E(m+1)=∑_{k=0..m} binom(m,k)*D(k)
 * A(m+1)=∑_{k=0..m} binom(m,k)*E(k)
 * Then a(n)=A(n). */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

seq_t a(idx_t n)
{
    /* Handle the trivial case */
    if (n == (idx_t)0) return (seq_t)1;

    idx_t size = n + (idx_t)1;   /* array size needed */

#ifdef __cplusplus
    /* C++: use vectors (proper construction/destruction) */
    std::vector<seq_t> A(size), B(size), C(size), D(size), E(size), row(size);
#else
    /* C: use malloc (seq_t is a simple integer type in C) */
    seq_t *A = (seq_t*)malloc(size * sizeof(seq_t));
    seq_t *B = (seq_t*)malloc(size * sizeof(seq_t));
    seq_t *C = (seq_t*)malloc(size * sizeof(seq_t));
    seq_t *D = (seq_t*)malloc(size * sizeof(seq_t));
    seq_t *E = (seq_t*)malloc(size * sizeof(seq_t));
    seq_t *row = (seq_t*)malloc(size * sizeof(seq_t));
    if (!A || !B || !C || !D || !E || !row) {
        /* Allocation failed – clean up and return 0 */
        free(A); free(B); free(C); free(D); free(E); free(row);
        return (seq_t)0;
    }
#endif

    /* Initial values: A[0] = 1, others 0 */
#ifdef __cplusplus
    A[0] = (seq_t)1;  B[0] = C[0] = D[0] = E[0] = (seq_t)0;
#else
    A[0] = (seq_t)1;  B[0] = C[0] = D[0] = E[0] = (seq_t)0;
#endif

    /* Iterate from m = 0 up to n‑1 to compute A[n] */
    for (idx_t m = (idx_t)0; m < n; ++m) {
        /* Build binomial coefficients for row m: \binom{m}{k} */
#ifdef __cplusplus
        row[0] = (seq_t)1;
        for (idx_t k = (idx_t)1; k <= m; ++k) {
            seq_t num = row[k - 1] * (seq_t)(m - k + (idx_t)1);
            row[k] = num / (seq_t)k;
        }
#else
        row[0] = (seq_t)1;
        for (idx_t k = (idx_t)1; k <= m; ++k) {
            seq_t num = row[k - 1] * (seq_t)(m - k + (idx_t)1);
            row[k] = num / (seq_t)k;
        }
#endif

        seq_t sumA = (seq_t)0, sumB = (seq_t)0;
        seq_t sumC = (seq_t)0, sumD = (seq_t)0, sumE = (seq_t)0;

        for (idx_t k = (idx_t)0; k <= m; ++k) {
            seq_t bin = row[k];
            sumB += bin * ( (idx_t)0==0?A[k]:A[k] );  /* A, B, C, D, E access */
            sumC += bin * ( (idx_t)0==0?B[k]:B[k] );
            sumD += bin * ( (idx_t)0==0?C[k]:C[k] );
            sumE += bin * ( (idx_t)0==0?D[k]:D[k] );
            sumA += bin * ( (idx_t)0==0?E[k]:E[k] );
        }

#ifdef __cplusplus
        A[m + (idx_t)1] = sumA;
        B[m + (idx_t)1] = sumB;
        C[m + (idx_t)1] = sumC;
        D[m + (idx_t)1] = sumD;
        E[m + (idx_t)1] = sumE;
#else
        A[m + (idx_t)1] = sumA;
        B[m + (idx_t)1] = sumB;
        C[m + (idx_t)1] = sumC;
        D[m + (idx_t)1] = sumD;
        E[m + (idx_t)1] = sumE;
#endif
    }

    seq_t result = (idx_t)0;
#ifdef __cplusplus
    result = A[n];
#else
    result = A[n];
    /* Free allocated memory */
    free(A); free(B); free(C); free(D); free(E); free(row);
#endif
    return result;
}
