/*
 * A269945  Triangle read by rows. Stirling set numbers of order 2, T(n, n) =
 *          1, T(n, k) = 0 if k < 0 or k > n, otherwise T(n, k) = T(n-1, k-1)
 *          + k^2*T(n-1, k), for 0 <= k <= n.
 * https://oeis.org/A269945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the row containing index n (i.e.
 *            r(r+1)/2 ≤ n < (r+1)(r+2)/2) and k = n - r(r+1)/2. The triangle
 *            T is defined by T(0,0) = 1 T(n,0) = 0 for n > 0 T(n,n) = 1 for
 *            all n T(n,k) = T(n-1,k-1) + k^2 * T(n-1,k) for 0 < k < n
 * Range:     u8 n <= 22, u16 n <= 38, u32 n <= 81, u64 n <= 176,
 *            u128 n <= 442, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* allocate two rows of maximum size r+1 */
    size_t sz = (size_t)(r + 1);
    seq_t *prev = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(sz, sizeof(seq_t));

    prev[0] = (seq_t)1;             /* T(0,0) */
    if (r == 0) {
        seq_t result = prev[0];
        free(prev);
        free(curr);
        return result;
    }

    /* build rows 1..r */
    for (idx_t i = 1; i <= r; ++i) {
        curr[i] = (seq_t)1;         /* T(i,i) = 1 */
        for (idx_t k = 0; k <= i - 1; ++k) {
            seq_t part1 = (k == 0) ? 0 : prev[k - 1];
            seq_t part2 = ((seq_t)k * (seq_t)k) * prev[k];
            curr[k] = part1 + part2;     /* T(i,k) */
        }
        /* swap rows for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* compute k index within row r */
    idx_t k = n - r * (r + 1) / 2;
    seq_t result = prev[k];

    free(prev);
    free(curr);
    return result;
}
