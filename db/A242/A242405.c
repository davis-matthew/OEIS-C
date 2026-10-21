/*
 * A242405  Expansion of (b(q) / b(q^2))^2 in powers of q where b() is a cubic
 *          AGM theta function.
 * https://oeis.org/A242405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the coefficient of x^n in (product_{k>0} ((1 -
 *            x^(2k-1))^3 / (1 - x^(6k-3))) )^2, i.e. a(n) = sum_{j=0}^{n}
 *            b(j) * b(n-j) where b(n) = coefficient of x^n in product_{k>0}
 *            (1 - x^(2k-1))^3 / (1 - x^(6k-3)). The function below computes
 *            a(n) directly
 * Range:     i8 n <= 6, i16 n <= 21, i32 n <= 75, i64 n <= 273,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Convert n to a regular integer type for array size */
    size_t N = (size_t)n;
    size_t i, j;

    /* Allocate arrays: A holds current product coefficients,
       H is temporary array for 1/(1 - x^(3m)) expansion,
       B holds new coefficients after applying the factor for m. */
    seq_t *A = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *H = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *B = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!A || !H || !B) return 0; /* allocation failure */

    A[0] = 1; /* initial product is 1 */

    /* Iterate over odd m = 1,3,5,... up to N */
    for (j = 1; j <= N; j += 2)
    {
        /* Copy current product A into H */
        for (i = 0; i <= N; ++i) H[i] = A[i];

        /* Compute H = H * (1 + x^(3m) + x^(6m) + ...)  = H / (1 - x^(3m)) */
        size_t step = 3 * j;
        for (i = step; i <= N; ++i)
            H[i] += H[i - step];

        /* Start new coefficients B as copy of H */
        for (i = 0; i <= N; ++i) B[i] = H[i];

        /* Apply the factor (1 - 3x^m + 3x^(2m) - x^(3m)) */
        if (j <= N)
        {
            for (i = j; i <= N; ++i)
                B[i] -= (seq_t)3 * H[i - j];
        }
        if (2 * j <= N)
        {
            for (i = 2 * j; i <= N; ++i)
                B[i] += (seq_t)3 * H[i - 2 * j];
        }
        if (3 * j <= N)
        {
            for (i = 3 * j; i <= N; ++i)
                B[i] -= H[i - 3 * j];
        }

        /* Prepare for next iteration: A <- B */
        seq_t *tmp = A;
        A = B;
        B = tmp;
        /* H will be overwritten in the next loop */
    }

    /* Compute convolution square: a(n) = sum_{k=0}^{n} A[k] * A[n-k] */
    seq_t result = 0;
    for (i = 0; i <= N; ++i)
    {
        seq_t prod = A[i] * A[N - i];
        result += prod;
    }

    /* Free memory */
    free(A);
    free(H);
    free(B);

    return result;
}
