/*
 * A294003  a(n) = n! * [x^n] exp(((1+x)^(n+1) - 1)/(n+1)).
 * https://oeis.org/A294003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * [x^n] exp(((1+x)^(n+1) - 1)/(n+1))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    /* Allocate arrays for the Bell polynomial coefficients.
       Use new/delete when compiled as C++ to invoke constructors
       for arbitrary‑precision integer classes. */
#ifdef __cplusplus
    seq_t *a_arr = new seq_t[n + 1];
    seq_t *B     = new seq_t[n + 1];
#else
    seq_t *a_arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *B     = (seq_t *)malloc((n + 1) * sizeof(seq_t));
#endif
    if (!a_arr || !B) {
#ifdef __cplusplus
        delete[] a_arr;
        delete[] B;
#else
        free(a_arr);
        free(B);
#endif
        return (seq_t)0;            /* allocation error */
    }

    /* Compute auxiliary values a_k = (k-1)! * binom(n, k-1).
       a_arr[0] is unused. */
    seq_t fac = (seq_t)1;           /* (k-1)!      */
    seq_t C   = (seq_t)1;           /* C(n,0)      */
    a_arr[0]  = (seq_t)0;           /* unused       */
    for (idx_t k = 1; k <= n; ++k) {
        a_arr[k] = fac * C;         /* a_k          */
        fac *= (seq_t)k;            /* (k)! for next step */
        /* update C to C(n,k) for next iteration */
        seq_t tmp = C * (seq_t)(n - (k - 1));
        C = tmp / (seq_t)k;         /* exact division */
    }

    /* Bell polynomial recurrence:
         B_0 = 1;
         B_m = Σ_{k=1..m} binom(m-1,k-1) * a_k * B_{m-k}   for m ≥ 1   */
    B[0] = (seq_t)1;
    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;                 /* binom(m-1,0) */
        for (idx_t k = 1; k <= m; ++k) {
            seq_t term = bin;
            term *= a_arr[k];
            term *= B[m - k];
            sum += term;
            if (k < m) {                     /* update binom(m-1,k) */
                seq_t tmp = bin * (seq_t)(m - k);
                bin = tmp / (seq_t)k;         /* exact division */
            }
        }
        B[m] = sum;
    }

    seq_t result = B[n];     /* a(n) = B_n */

    /* Clean up */
#ifdef __cplusplus
    delete[] a_arr;
    delete[] B;
#else
    free(a_arr);
    free(B);
#endif

    return result;
}
