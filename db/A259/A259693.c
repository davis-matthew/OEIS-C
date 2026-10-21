/*
 * A259693  a(n) = Sum_{k=1..n-1} k^5*sigma(k)*sigma(n-k).
 * https://oeis.org/A259693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n-1} k^5 * sigma(k) * sigma(n-k) where sigma(m)
 *            = sum_{d|m} d
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 23, u64 n <= 371, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* seq_t and idx_t are provided by the build environment.
   Assume seq_t supports the usual arithmetic operators. */

seq_t a(idx_t n)
{
    static idx_t maxN = 0;
    static seq_t *sigma = NULL;
    static seq_t *A      = NULL;

    /* Build or extend the tables up to n. */
    if (n > maxN) {
        idx_t newN = n;

        seq_t *newSigma;
        seq_t *newA;

#ifdef __cplusplus
        newSigma = new seq_t[newN + 1];
        newA     = new seq_t[newN + 1];
#else
        newSigma = (seq_t *)malloc((newN + 1) * sizeof(seq_t));
        newA     = (seq_t *)malloc((newN + 1) * sizeof(seq_t));
#endif

        /* Initialize to zero. */
        for (idx_t i = 0; i <= newN; ++i)
            newSigma[i] = (seq_t)0;

        /* Compute sigma(j) for all j ≤ newN. */
        for (idx_t i = 1; i <= newN; ++i) {
            for (idx_t j = i; j <= newN; j += i)
                newSigma[j] += (idx_t)i;
        }

        /* Compute A[k] = k^5 * sigma(k). */
        for (idx_t k = 1; k <= newN; ++k) {
            seq_t kseq = (seq_t)k;
            seq_t k5   = kseq * kseq * kseq * kseq * kseq;
            newA[k] = k5 * newSigma[k];
        }
        newA[0] = (seq_t)0;  /* unused, but set for completeness */

        /* Free old tables if they exist. */
        if (sigma) {
#ifdef __cplusplus
            delete[] sigma;
#else
            free(sigma);
#endif
        }
        if (A) {
#ifdef __cplusplus
            delete[] A;
#else
            free(A);
#endif
        }

        /* Commit new tables. */
        sigma = newSigma;
        A    = newA;
        maxN = newN;
    }

    /* Compute the sum for the requested n. */
    seq_t result = (seq_t)0;
    for (idx_t k = 1; k < n; ++k) {
        result += A[k] * sigma[n - k];
    }

    return result;
}
