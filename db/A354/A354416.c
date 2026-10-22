/*
 * A354416  Expansion of e.g.f. (1 - log(1-x))^x.
 * https://oeis.org/A354416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k * A089064(k-1) * binomial(n-1,k-1) *
 *            a(n-k) * where A089064(m) is the sequence defined by A089064(m)
 *            = m! * coeff_{x^m} log(1 - log(1-x)) * * The implementation
 *            precomputes the required A089064 values using an efficient
 *            recurrence, * then evaluates the recurrence for a(n). A...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case */
    if (n == 0)
        return (seq_t)1;

    /* Convert to a C type for indexing */
    idx_t N = n;

    /* Allocate space for a[0..N], A[0..N], D[0..N-1] */
    seq_t *a_arr   = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *A_arr   = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *D_arr   = (seq_t *)malloc(N * sizeof(seq_t));

    if (!a_arr || !A_arr || !D_arr) {
        /* In the unlikely event of allocation failure, abort
           (the caller does not handle errors, so we simply
           return 0). */
        return (seq_t)0;
    }

    /* ----- Compute D[0..N-1] ----- */
    D_arr[0] = (seq_t)1;          /* D[0] = 1 */
    for (idx_t m = 1; m < N; ++m) {
        seq_t ff = (seq_t)1;    /* falling factorial product */
        seq_t d  = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k) {
            ff *= (seq_t)(m - k + 1);               /* ff = m*(m-1)*...*(m-k+1) */
            seq_t term = (ff / (seq_t)k) * D_arr[m - k];
            d -= term;
        }
        D_arr[m] = d;
    }

    /* ----- Compute A[0..N-1] ----- */
    A_arr[0] = (seq_t)0;          /* A[0] = 0 = A089064(0) */
    if (N >= 1)
        A_arr[1] = (seq_t)1;      /* A[1] = 1 = A089064(1) */
    for (idx_t M = 1; M < N; ++M) {
        /* A[M+1] = M * A[M] + D[M]   (recurrence derived above) */
        A_arr[M + 1] = (seq_t)M * A_arr[M] + D_arr[M];
    }

    /* ----- Compute a[0..N] ----- */
    a_arr[0] = (seq_t)1;          /* a(0) = 1 */
    for (idx_t i = 1; i <= N; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t comb  = binomial(i - 1, k - 1);          /* binomial(i-1,k-1) */
            seq_t term  = comb * a_arr[i - k];            /* comb * a(n-k) */
            term *= ((seq_t)k * A_arr[k - 1]);             /* * k * A(k-1) */
            sum += term;
        }
        a_arr[i] = sum;
    }

    seq_t result = a_arr[N];

    /* Free allocated memory */
    free(a_arr);
    free(A_arr);
    free(D_arr);

    return result;
}
