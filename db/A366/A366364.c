/*
 * A366364  G.f. satisfies A(x) = (1 + x/A(x)^2)/(1 - x).
 * https://oeis.org/A366364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generating function: A(x) = (1 + x/A(x)^2)/(1 - x) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *A = NULL;            /* sequence values a[0..] */
    static seq_t *C = NULL;            /* convolution A*A values conv[0..] */
    static size_t cap = 0;             /* current allocated capacity (>= n+1) */
    static idx_t next = 0;             /* next index to compute */
    static seq_t triple_last = 0;      /* triple[n-1] = Σ_{i=0}^{n-1} a[i]*conv[n-1-i] */

    /* first call – initialise arrays for index 0 */
    if (!A) {
        cap = 1;
        A = (seq_t *)malloc(2 * sizeof(seq_t));   /* indices 0 .. cap */
        C = (seq_t *)malloc(2 * sizeof(seq_t));
        A[0] = 1;
        C[0] = 1;              /* conv[0] = a0*a0 = 1 */
        triple_last = 1;        /* triple[0] = a0*conv[0] = 1 */
        next = 1;               /* next unknown index */
    }

    /* enlarge arrays if needed */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap ? cap : 1;
        while ((size_t)n + 1 > newcap) newcap <<= 1;
        A = (seq_t *)realloc(A, (newcap + 1) * sizeof(seq_t));
        C = (seq_t *)realloc(C, (newcap + 1) * sizeof(seq_t));
        cap = newcap;
    }

    /* compute terms up to n */
    for (idx_t k = next; k <= n; ++k) {
        /* S2 = Σ_{i=1}^{k-1} a[i] * conv[k-i] */
        seq_t S2 = 0;
        for (idx_t i = 1; i < k; ++i) {
            S2 += A[i] * C[k - i];
        }

        /* a(k) = triple_last + (k==1?1:0) - S2 */
        seq_t ak = triple_last + ((k == 1) ? (seq_t)1 : (seq_t)0) - S2;

        /* S1 = Σ_{i=1}^{k-1} a[i] * a[k-i] (symmetrised) */
        seq_t S1 = 0;
        idx_t half = (k - 1) / 2;
        for (idx_t i = 1; i <= half; ++i) {
            S1 += A[i] * A[k - i] * (seq_t)2;
        }
        if ((k % 2) == 0) {          /* even k: add middle term once */
            seq_t mid = A[k / 2];
            S1 += mid * mid;
        }

        /* conv[k] = 2*a(k) + S1 */
        C[k] = ak * (seq_t)2 + S1;

        /* triple_last = triple[k] = C[k] + S2 + a(k) */
        triple_last = C[k] + S2 + ak;

        /* store a(k) */
        A[k] = ak;
    }

    next = n + 1;
    return A[n];
}
