/*
 * A325751  First term of n-th difference sequence of (floor(r*k)), r =
 *          log(2), k >= 0.
 * https://oeis.org/A325751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = Σ_{j=0}^{n} (-1)^{n-j} * C(n,j) * floor(log(2) * j)
 * Range:     i8 n <= 11, i16 n <= 18, i32 n <= 38, i64 n <= 67, i128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence gives the first term of the n‑th forward difference of       */
/* f(k) = floor(log(2) * k).  The implementation uses an O(n²) difference  */
/* construction which avoids large binomial coefficients and all arithmetic  */
/* is performed with the seq_t type provided by the build.                 */

#include <stdlib.h>

/* Natural logarithm of 2 (double precision).  The values we compute are   */
/* small enough (< 10 000 * 0.7 ≈ 7 000) so truncating a double to a     */
/* long long yields the correct floor.  No <math.h> functions are used.    */
static const double LOG2 = 0.6931471805599453094172321214581765687;

/* Compute the n‑th term of the sequence, for n >= 1.                       */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* offset 1: a(1) = f(0) = 0 */

    /* Allocate array of size n+1 for the values f(0)…f(n). */
    seq_t *arr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!arr) return (seq_t)0;           /* allocation failure → 0 */

    /* Fill arr with f(k) = floor(log(2) * k). */
    for (idx_t k = 0; k <= n; ++k) {
        long long v = (long long)(LOG2 * (double)k);   /* truncation == floor for k >= 0 */
        arr[k] = (seq_t)v;
    }

    /* Perform successive forward differences in place.  After d steps   */
    /* the first n-d+1 entries contain Δ^d f(0), Δ^d f(1), …           */
    for (idx_t d = 1; d <= n; ++d) {
        idx_t limit = n - d + 1;                 /* number of entries still valid */
        for (idx_t i = 0; i < limit; ++i) {
            arr[i] = arr[i + 1] - arr[i];
        }
    }

    seq_t result = arr[0];                      /* a(n) = Δ^n f(0)               */
    free(arr);
    return result;
}
