/*
 * A372013  G.f. A(x) satisfies A(x) = 1/( 1 + x*(1 - 9*x*A(x))^(1/3) ).
 * https://oeis.org/A372013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) computed from the generating function A(x) = 1/( 1 + x*(1 -
 *            9*x*A(x))^(1/3) ). Sequence values are the integer coefficients
 *            of A(x)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Computes the n-th term of sequence A372013. 
   a(0) = 1, a(1) = -1, a(2) = 4, …  */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* Allocate dynamic arrays for coefficients up to n. */
    seq_t *a_arr   = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *b_arr   = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *conv2   = (seq_t*)malloc((2 * n + 1) * sizeof(seq_t));

    /* Initialize arrays to zero. */
    for (idx_t i = 0; i <= n; ++i) {
        a_arr[i] = 0;
        b_arr[i] = 0;
    }
    for (idx_t i = 0; i <= 2 * n; ++i) {
        conv2[i] = 0;
    }

    /* Base values. */
    a_arr[0] = (seq_t)1;
    b_arr[0] = (seq_t)1;
    conv2[0] = (seq_t)1;      /* b0*b0 */

    /* Build coefficients iteratively. */
    for (idx_t k = 1; k <= n; ++k) {
        /* Compute a_k = - sum_{i=0}^{k-1} a_i * b_{k-1-i} */
        seq_t sumA = 0;
        for (idx_t i = 0; i < k; ++i) {
            sumA += a_arr[i] * b_arr[k - 1 - i];
        }
        a_arr[k] = -sumA;

        /* Compute T_k = sum_{i=0}^{k-1} b_i * conv2[k - i] */
        seq_t T = 0;
        for (idx_t i = 0; i < k; ++i) {
            T += b_arr[i] * conv2[k - i];
        }

        /* Compute b_k = (-9*a_{k-1} - T)/3 */
        seq_t numer = -((seq_t)9 * a_arr[k - 1]) - T;
        b_arr[k] = numer / (seq_t)3;

        /* Update conv2 to include new b_k values.
           conv2[idx] = sum_{i+j=idx} b_i*b_j for i,j <= current. */
        for (idx_t j = 0; j < k; ++j) {
            conv2[k + j] += b_arr[k] * b_arr[j] * (seq_t)2;
        }
        conv2[2 * k] += b_arr[k] * b_arr[k];
    }

    /* Result. */
    seq_t result = a_arr[n];

    /* Free allocated memory. */
    free(a_arr);
    free(b_arr);
    free(conv2);

    return result;
}
