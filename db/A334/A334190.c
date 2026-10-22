/*
 * A334190  a(n) = exp(1/2) * Sum_{k>=0} (2*k + 1)^n / ((-2)^k * k!).
 * https://oeis.org/A334190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = exp(1/2) * Sum_{k>=0} (2*k + 1)^n / ((-2)^k * k!)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 14, i64 n <= 23, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Allocate array of a[0..n] */
    size_t len = (size_t)(n + 1);
#ifdef __cplusplus
    seq_t *arr = new seq_t[len];
#else
    seq_t *arr = (seq_t*)malloc(len * sizeof(seq_t));
#endif
    if (!arr) { /* allocation failed */
#ifdef __cplusplus
        delete[] arr;
#else
        free(arr);
#endif
        return (seq_t)0;
    }

    arr[0] = (seq_t)1;

    for (idx_t m = 0; m < n; ++m) {
        seq_t sum = 0;
        seq_t C = 1;          /* binomial coefficient C(m, k) */
        seq_t pow2 = 2;        /* 2^k for k = 1 */

        for (idx_t k = 1; k <= m; ++k) {
            /* Update binomial coefficient: C(m, k) = C(m, k-1) * (m-k+1) / k */
            C = (C * (seq_t)(m - k + 1)) / (seq_t)k;

            seq_t temp = C * arr[m - k];  /* C * a[m-k] */
            temp = temp * pow2;           /* * 2^k */
            temp = -temp;                 /* multiply by -1  (since the recurrence has -2^k) */
            sum += temp;

            pow2 = pow2 * 2;              /* next power of two */
        }
        arr[m + 1] = sum;
    }

    seq_t result = arr[n];

#ifdef __cplusplus
    delete[] arr;
#else
    free(arr);
#endif
    return result;
}
