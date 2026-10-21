/*
 * A278075  Coefficients of the signed Fubini polynomials in ascending order,
 *          F_n(x) = Sum_{k=0..n} (-1)^n*Stirling2(n,k)*k!*(-x)^k.
 * https://oeis.org/A278075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = T(r,k) with T(n,k)=(-1)^(n-k)*Stirling2(n,k)*k! ; r =
 *            floor((sqrt(8*n+1)-1)/2) , k = n - r*(r+1)/2
 * Range:     i8 n <= 17, i16 n <= 39, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* ---------- determine the row r and column k ---------- */
    idx_t r = (idx_t)((sqrt((double)n * 8.0 + 1.0) - 1.0) / 2.0);
    /* correct possible rounding errors */
    while (r * (r + 1) / 2 > n) --r;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    idx_t k = n - r * (r + 1) / 2;

    /* ---------- handle the trivial top row ---------- */
    if (r == 0) {
        return (seq_t)1;
    }

    /* ---------- build rows up to r, keeping only the previous row ---------- */
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t));   /* row 0 */
    prev[0] = (seq_t)1;

    for (idx_t i = 1; i <= r; ++i) {
        size_t len = (size_t)(i + 1);
        seq_t *curr = (seq_t *)malloc(len * sizeof(seq_t));

        for (idx_t j = 0; j <= i; ++j) {
            if (j == 0) {
                curr[j] = (seq_t)0;          /* by definition in the recurrence */
            } else {
                seq_t t1 = prev[j - 1];
                seq_t t2 = (j < i) ? prev[j] : (seq_t)0;
                curr[j] = (t1 - t2) * (seq_t)j;   /* T(i,j) = j*(T(i-1,j-1)-T(i-1,j)) */
            }
        }

        free(prev);
        prev = curr;
    }

    seq_t result = prev[k];
    free(prev);
    return result;
}
