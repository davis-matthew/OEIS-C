/*
 * A394122  G.f. A(x) satisfies
 *          A(x)^6-A(x)^5+5x*A(x)^4-(x^2+2x)*A(x)^3+5x^2*A(x)^2-x^2*A(x)+x^3=0.
 * https://oeis.org/A394122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: A(x)^6 - A(x)^5 + 5*x*A(x)^4 - (x^2+2*x)*A(x)^3 + 5*x^2*A(x)^2
 *          - x^2*A(x) + x^3 = 0
 *
 * The coefficients a(n) are obtained from the formal power series of A(x).
 * Recurrence (derived from the coefficient of x^{n+2}):
 *
 *    a(n) = p6[n+2] - p5[n+2] + 5*p4[n+1] - p3[n] - 2*p3[n+1]
 *           + 5*p2[n] + (n==1 ? 1 : 0)
 *
 * where p_k[m] is the coefficient of x^m in A(x)^k.
 *
 * Initial values: a(0)=0, a(1)=1, a(2)=3, a(3)=15, a(4)=88, ...
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* Allocate arrays for a(0..n+2) and the powers of A(x) up to needed index.
     * We allocate n+3 elements (indices 0 .. n+2). */
    size_t size = (size_t)(n + 3);
    seq_t *A   = (seq_t *)malloc(size * sizeof(seq_t));
    seq_t *P2  = (seq_t *)malloc(size * sizeof(seq_t));
    seq_t *P3  = (seq_t *)malloc(size * sizeof(seq_t));
    seq_t *P4  = (seq_t *)malloc(size * sizeof(seq_t));
    seq_t *P5  = (seq_t *)malloc(size * sizeof(seq_t));
    seq_t *P6  = (seq_t *)malloc(size * sizeof(seq_t));

    if (!A || !P2 || !P3 || !P4 || !P5 || !P6) {
        /* out of memory – return 0 as a safe fallback */
        return 0;
    }

    /* initialise all arrays to zero */
    memset(A,  0, size * sizeof(seq_t));
    memset(P2, 0, size * sizeof(seq_t));
    memset(P3, 0, size * sizeof(seq_t));
    memset(P4, 0, size * sizeof(seq_t));
    memset(P5, 0, size * sizeof(seq_t));
    memset(P6, 0, size * sizeof(seq_t));

    A[0] = 0;      /* a(0) = 0 */

    for (idx_t i = 1; i <= n; ++i) {
        /* ---- compute P2[i] ---- */
        seq_t sum = 0;
        for (idx_t j = 1; j < i; ++j)
            sum += A[j] * A[i - j];
        P2[i] = sum;

        /* ---- compute P3[i] ---- */
        if (i >= 3) {
            sum = 0;
            for (idx_t j = 1; j <= i - 2; ++j)
                sum += A[j] * P2[i - j];
            P3[i] = sum;
        } else {
            P3[i] = 0;
        }

        /* ---- compute P3[i+1] ---- */
        if (i >= 2) {
            sum = 0;
            for (idx_t j = 1; j <= i - 1; ++j)
                sum += A[j] * P2[i + 1 - j];
            P3[i + 1] = sum;
        } else {
            P3[i + 1] = 0;
        }

        /* ---- compute P4[i+1] ---- */
        if (i >= 3) {
            sum = 0;
            for (idx_t j = 1; j <= i - 2; ++j)
                sum += A[j] * P3[i + 1 - j];
            P4[i + 1] = sum;
        } else {
            P4[i + 1] = 0;
        }

        /* ---- compute P5[i+2] ---- */
        if (i >= 3) {
            sum = 0;
            for (idx_t j = 1; j <= i - 2; ++j)
                sum += A[j] * P4[i + 2 - j];
            P5[i + 2] = sum;
        } else {
            P5[i + 2] = 0;
        }

        /* ---- compute P6[i+2] ---- */
        if (i >= 4) {
            sum = 0;
            for (idx_t j = 1; j <= i - 3; ++j)
                sum += A[j] * P5[i + 2 - j];
            P6[i + 2] = sum;
        } else {
            P6[i + 2] = 0;
        }

        /* ---- compute a(i) ---- */
        seq_t term = P6[i + 2];
        term -= P5[i + 2];
        term += 5 * P4[i + 1];
        term -= P3[i];
        term -= 2 * P3[i + 1];
        term += 5 * P2[i];
        if (i == 1) term += 1;   /* the δ_{i,1} term */
        A[i] = term;
    }

    seq_t result = A[n];

    free(A);
    free(P2);
    free(P3);
    free(P4);
    free(P5);
    free(P6);

    return result;
}
