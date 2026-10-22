/*
 * A350366  a(n) = [x^n] Product_{k=1..n} (1 + k*x)/(1 - k*x).
 * https://oeis.org/A350366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = [x^n] Product_{k=1..n} (1 + k*x)/(1 - k*x)
 * (OEIS A350366)
 */
#include <stdlib.h>

/* Computes a(n) for OEIS A350366.  Works for any seq_t/idx_t pair
 * provided by the build.  All operations are purely integral. */
seq_t a(idx_t n)
{
    /* Handle the trivial case */
    if (n == (idx_t)0)
        return (seq_t)1;

    size_t N = (size_t)n;                     /* maximum degree needed   */
    /* Allocate two arrays of length N+1 to hold current coefficients
       and the intermediate result after multiplying by (1 + k*x). */
    seq_t *coeff = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *temp   = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!coeff || !temp) {                /* allocation failure */
        free(coeff);
        free(temp);
        return (seq_t)0;
    }

    /* Initialize the series to 1 (empty product). */
    for (size_t i = 0; i <= N; ++i)
        coeff[i] = (seq_t)0;
    coeff[0] = (seq_t)1;

    /* Iteratively multiply by each factor (1 + k*x)/(1 - k*x). */
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t kseq = (seq_t)k;            /* cast once for speed */

        /* Step 1: multiply by (1 + k*x).  The result is stored in temp[]. */
        /* temp[m] = coeff[m] + k*coeff[m-1] (with coeff[-1] treated as 0). */
        temp[0] = coeff[0];                /* coeff[-1] is 0 */
        for (size_t m = 1; m <= N; ++m)
            temp[m] = coeff[m] + kseq * coeff[m - 1];

        /* Step 2: multiply the intermediate series by (1 - k*x)^(-1).
           This convolution can be done with the recurrence
               new[m] = new[m-1]*k + temp[m],  new[-1] = 0.          */
        seq_t prev = (seq_t)0;             /* new[-1] */
        for (size_t m = 0; m <= N; ++m)
        {
            seq_t cur = prev * kseq + temp[m];
            coeff[m] = cur;
            prev = cur;
        }
    }

    seq_t result = coeff[N];                 /* coefficient of x^n          */
    free(coeff);
    free(temp);
    return result;
}
