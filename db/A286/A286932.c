/*
 * A286932  Square array A(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of continued fraction 1/(1 + k*x/(1 +
 *          k*x^2/(1 + k*x^3/(1 + k*x^4/(1 + k*x^5/(1 + ...)))))).
 * https://oeis.org/A286932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 39, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the nth term of the triangle A(n,k) read along antidiagonals.
 * Each entry A(i,k) is the coefficient of x^i in the continued fraction
 * 1/(1 + k*x/(1 + k*x^2/(1 + k*x^3/(1 + …)))). The function below
 * computes that coefficient by iteratively inverting the power series
 * 1 + k*x^i * f_{i+1}.
 */

#include <stddef.h>   /* for size_t */

#define MAX_DEG 150   /* > maximum degree for n up to ~10000 */

/* Return coefficient of x^deg in column k of the continued fraction. */
static seq_t coeff_col(idx_t k, idx_t deg)
{
    if (deg == 0)
        return (seq_t)1;

    seq_t curr[MAX_DEG], next[MAX_DEG];
    size_t d;

    /* start from the tail: f_{∞} = 1 */
    curr[0] = (seq_t)1;
    for (d = 1; d <= (size_t)deg; ++d)
        curr[d] = (seq_t)0;

    /* depth‑by‑depth inversion: compute f_i from f_{i+1} */
    for (int level = (int)deg; level >= 1; --level)
    {
        next[0] = (seq_t)1;
        for (int m = 1; m <= (int)deg; ++m)
        {
            seq_t sum = (seq_t)0;
            for (int t = level; t <= m; ++t)
            {
                seq_t term = (seq_t)k * curr[t - level];
                sum += term * next[m - t];
            }
            next[m] = -sum;
        }
        /* copy next → curr for the next iteration */
        for (d = 0; d <= (size_t)deg; ++d)
            curr[d] = next[d];
    }

    return curr[(size_t)deg];
}

/* Main sequence accessor required by the framework. */
seq_t a(idx_t n)
{
    /* locate (i,j) such that i + j = t and t*(t+1)/2 <= n < (t+1)(t+2)/2 */
    idx_t t = 0;
    while (((t + 1ULL) * (t + 2ULL)) / 2ULL <= n)
        ++t;

    idx_t sumPrev = t * (t + 1ULL) / 2ULL;
    idx_t i = n - sumPrev;          /* row: power of x   */
    idx_t j = t - i;                /* column: k         */

    if (j == 0)                     /* column 0 */
        return (i == 0) ? (seq_t)1 : (seq_t)0;

    if (i == 0)                     /* first row of any column */
        return (seq_t)1;

    return coeff_col(j, i);
}
