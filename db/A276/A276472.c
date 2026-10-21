/*
 * A276472  Modified Pascal's triangle read by rows: T(n,k) = T(n-1,k) +
 *          T(n-1,k-1), 1<k<n. T(n,1) = T(n-1,1) + T(n,2), n>2. T(n,n) =
 *          T(n,n-1) + T(n-1,n-1), n>1. T(1,1) = 1, T(2,1) = 1. n>=1.
 * https://oeis.org/A276472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r satisfies r*(r+1)/2 >= n > (r-1)*r/2 and k
 *            = n - (r-1)*r/2. Here T(n,k) is defined by T(n,k) = T(n-1,k) +
 *            T(n-1,k-1) for 1 < k < n T(n,1) = T(n-1,1) + T(n,2) for n > 2
 *            T(n,n) = T(n,n-1) + T(n-1,n-1) for n > 1 with T(1,1)=1 and
 *            T(2,1)=1. The sequence enumerates the triangle r...
 * Range:     u8 n <= 28, u16 n <= 90, u32 n <= 300, u64 n <= 1128,
 *            u128 n <= 4371, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset starts at 1 */
    /* find row r with r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    idx_t prevCount = ((r - 1) * r) / 2;       /* elements before row r */
    idx_t k = n - prevCount;                  /* column in row r (1‑based) */

    /* allocate rows up to r */
    seq_t *prev = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));

    /* base row 1 */
    prev[1] = 1;
    if (r == 1) {
        free(prev);
        free(curr);
        return 1;
    }

    /* row 2 */
    curr[1] = 1;
    curr[2] = curr[1] + prev[1];   /* 1 + 1 = 2 */
    if (r == 2) {
        seq_t res = (k == 1) ? curr[1] : curr[2];
        free(prev);
        free(curr);
        return res;
    }

    /* swap prev and curr ready for next iteration */
    seq_t *tmp = prev; prev = curr; curr = tmp;

    /* compute rows 3 .. r */
    for (idx_t row = 3; row <= r; ++row) {
        /* interior entries */
        for (idx_t j = 2; j < row; ++j) {
            curr[j] = prev[j] + prev[j - 1];
        }
        /* right border */
        curr[row] = curr[row - 1] + prev[row - 1];
        /* left border uses interior entry curr[2] just computed */
        curr[1] = prev[1] + curr[2];
        /* prepare for next iteration */
        tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[k];
    free(prev);
    free(curr);
    return result;
}
