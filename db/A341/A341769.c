/*
 * A341769  Number of connected components of Euclidean n-space when the
 *          hyperplanes x_i+x_j=1, x_i=0, x_i=1 (1 <= i < j <= n) are deleted.
 * https://oeis.org/A341769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A000670(0) = 1 A000670(m) = sum_{k=0}^{m-1} binom(m,k) *
 *            A000670(k) a(1) = 3 a(n) = 2 * ( A000670(n+1) - (n+1)*A000670(n)
 *            + n*A000670(n-1) ) for n >= 2 (All operations are performed in
 *            seq_t. All arithmetic is exact.)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *A = NULL;      /* array of A000670 values */
static idx_t A_len = 0;      /* count of computed elements */

/* Ensure that A[0..n] are computed */
static void ensure_A(idx_t n)
{
    if (A_len > n) return;          /* already computed */

    /* resize storage */
    size_t new_len = (size_t)(n + 1);
    if (A_len == 0) {
        A = (seq_t*)malloc(new_len * sizeof(seq_t));
    } else {
        A = (seq_t*)realloc(A, new_len * sizeof(seq_t));
    }

    /* initialize from current length up to new_len-1 */
    if (A_len == 0) {
        A[0] = (seq_t)1;            /* A000670(0) = 1 */
        A_len = 1;
    }
    for (idx_t m = A_len; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;     /* binom(m,0) = 1 */
        for (idx_t k = 0; k < m; ++k) {
            sum = sum + binom * A[k];
            binom = binom * (m - k);        /* binom(m,k+1) = binom(m,k) * (m-k)/(k+1) */
            binom = binom / (k + 1);
        }
        A[m] = sum;
    }
    A_len = new_len;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;

    /* We need A[0..n+1] */
    ensure_A(n + 1);

    seq_t term = A[n + 1];
    term = term - (seq_t)(n + 1) * A[n];
    term = term + (seq_t)n * A[n - 1];
    return term * (seq_t)2;
}
