/*
 * A380191  Triangle read by rows: Riordan array (2 - D(x), x * D(x)) where
 *          D(x) is g.f. of A001764.
 * https://oeis.org/A380191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 14, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A380191
 * Definition: list the entries of the Riordan array T(n,k) defined by
 *   a(n,k) = binomial(3*n-2*k, n-k) * (n*k+4*k-3*n) / ((3*n-2*k)*(2*n-k+1)),
 * for 0 <= k < n and a(n,n) = 1.
 * The sequence a(n) is obtained by reading the triangle row by row
 * (row 0 has 1 element, row 1 has 2, …).  Thus for a given index n
 * we determine the row r and column c:
 *   r = floor( (sqrt(8*n+1)-1)/2 )
 *   c = n - r*(r+1)/2
 * and return T(r,c).
 * This implementation uses the provided binomial function and integer arithmetic
 * only.  It works for any seq_t type (signed integers or arbitrary precision).
 */

#include <math.h>
#include "../utils/binomial.h"

static seq_t T_value(idx_t n, idx_t k)
{
    /* Return the entry T(n,k) of the Riordan triangle. */
    if (k == n) {
        return (seq_t)1;
    }
    /* Use the given closed formula. */
    seq_t bin = binomial((idx_t)(3*n - 2*k), (idx_t)(n - k));
    seq_t num_factor = (seq_t)n * (seq_t)k + 4*(seq_t)k - 3*(seq_t)n;
    seq_t numerator   = bin * num_factor;
    seq_t denominator = (seq_t)(3*n - 2*k) * (seq_t)(2*n - k + 1);
    return numerator / denominator;
}

seq_t a(idx_t n)
{
    /* Determine the row and column for index n. */
    double root = sqrt((double)(8 * n + 1));
    idx_t r = (idx_t)((root - 1.0) / 2.0);          /* floor */
    idx_t elements_before = r * (r + 1) / 2;        /* sum_{i=0}^{r-1} (i+1) */
    idx_t c = n - elements_before;
    return T_value(r, c);
}
