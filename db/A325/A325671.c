/*
 * A325671  First term of n-th difference sequence of (floor(k*r)), r =
 *          -sqrt(6), k >= 0.
 * https://oeis.org/A325671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 20, i32 n <= 34, i64 n <= 66, i128 n <= 130,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A325671
 *  First term of n-th difference sequence of (floor(k*(-sqrt(6))), k >= 0.
 *
 *  The sequence is obtained by computing the forward differences of
 *  b_k = floor(k*r) with r = -sqrt(6) and taking the first term of
 *  the n‑th order difference.  For n = 1 we return -3, for n ≥ 2 we
 *  compute Δ^(n‑1) b(0) by generating the first n differences of
 *  floor(k*sqrt(6)) and applying the finite difference operator.
 *
 *  The algorithm is O(n²) with n up to about 10⁴ and fits easily
 *  within the time limits.  All arithmetic is performed in the
 *  integer type seq_t, using the provided floor_sqrt function to
 *  avoid the use of floating point or external libraries.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n is 1‑based (n >= 1) */
    idx_t m = n - 1;                /* number of forward differences needed */

    /* Allocate array of length m+1 for the first differences d[0..m] */
    seq_t *d = (seq_t *)malloc((m + 1) * sizeof(seq_t));
    if (!d) return 0;               /* allocation failure - should not happen */

    /* The first difference d[0] = -ceil(sqrt(6)) = -(floor(sqrt(6))+1) = -3 */
    d[0] = -((seq_t)1 + floor_sqrt((seq_t)6));

    /* Compute the remaining differences using floor_sqrt(k*sqrt(6)) values */
    for (idx_t k = 1; k <= m; ++k) {
        seq_t floor_k       = floor_sqrt((seq_t)6 * k * k);          /* floor(k*sqrt6) */
        seq_t floor_kp1     = floor_sqrt((seq_t)6 * (k + 1) * (k + 1)); /* floor((k+1)*sqrt6) */
        d[k] = -(floor_kp1 - floor_k);
    }

    /* Apply the finite difference operator (m times) in place */
    for (idx_t t = 1; t <= m; ++t) {
        for (idx_t i = 0; i < m - t + 1; ++i) {
            d[i] = d[i + 1] - d[i];
        }
    }

    seq_t result = d[0];
    free(d);
    return result;
}
