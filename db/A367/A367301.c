/*
 * A367301  Triangular array T(n,k), read by rows: coefficients of strong
 *          divisibility sequence of polynomials p(1,x) = 1, p(2,x) = 3 + 3*x,
 *          p(n,x) = u*p(n-1,x) + v*p(n-2,x) for n >= 3, where u = p(2,x), v =
 *          1 - 2*x - x^2.
 * https://oeis.org/A367301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient obtained by reading the triangular array
 *            of coefficients of polynomials defined recursively by p(1,x) =
 *            1, p(2,x) = 3 + 3*x, p(n,x) = (3+3*x)*p(n-1,x) +
 *            (1-2*x-x^2)*p(n-2,x) (n ≥ 3). The sequence is read by rows: the
 *            first row contains 1, the second contains 3,3, the third...
 * Range:     u8 n <= 11, u16 n <= 37, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r and position k within that row for index n
       (1‑based).  Sum i from 1 until triangular number >= n. */
    idx_t sum = 0;
    idx_t r;
    for (r = 1; ; ++r) {
        sum += r;
        if (sum >= n) break;
    }
    idx_t prev_sum = sum - r;          /* sum of lengths of previous rows */
    idx_t k = n - prev_sum;            /* 1‑based position in row r */

    /* Handle the first two rows directly. */
    if (r == 1) return (seq_t)1;
    if (r == 2) return (seq_t)3;   /* both entries are 3 */

    /* Allocate arrays for the previous two rows and the current row. */
    seq_t *prev2 = (seq_t *)malloc(1 * sizeof(seq_t));   /* row n‑2 */
    seq_t *prev  = (seq_t *)malloc(2 * sizeof(seq_t));   /* row n‑1 */
    if (!prev2 || !prev) return (seq_t)0;  /* allocation failure (unlikely) */

    prev2[0] = (seq_t)1;        /* p(1,x) = 1 */
    prev[0]  = (seq_t)3;        /* p(2,x) = 3 + 3x */
    prev[1]  = (seq_t)3;
    idx_t prev2_len = 1;
    idx_t prev_len  = 2;

    /* Compute rows 3 .. r. */
    for (idx_t row = 3; row <= r; ++row) {
        idx_t cur_len = row;
        seq_t *curr = (seq_t *)malloc(cur_len * sizeof(seq_t));
        if (!curr) { free(prev2); free(prev); return (seq_t)0; }

        for (idx_t i = 0; i < cur_len; ++i) {
            seq_t P = 0;   /* positive part */
            if (i < prev_len) P += prev[i] * (seq_t)3;
            if (i >= 1)        P += prev[i-1] * (seq_t)3;
            if (i < prev2_len) P += prev2[i];

            seq_t N = 0;   /* negative part */
            if (i >= 1 && (i-1) < prev2_len) N += prev2[i-1] * (seq_t)2;
            if (i >= 2 && (i-2) < prev2_len) N += prev2[i-2];

            curr[i] = P - N;   /* safe: P ≥ N for these polynomials */
        }

        free(prev2);
        prev2 = prev;            /* shift rows */
        prev2_len = prev_len;
        prev = curr;
        prev_len = cur_len;
    }

    /* Return the required coefficient from the final row. */
    seq_t result = prev[k-1];

    free(prev2);
    free(prev);
    return result;
}
