/*
 * A366986  Square array T(n,k), n >= 1, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{d|n} binomial(d+k-1,k).
 * https://oeis.org/A366986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where T(i,j) = Sum_{d|i} binomial(d + j - 1, j).
 *            The indices (i,j) are obtained from n as follows: let m = n-1, d
 *            = floor((sqrt(8*m+1)-1)/2), total_prev = d*(d+1)/2, offset = m -
 *            total_prev, i = offset + 1, j = d - offset
 * Range:     u8 n <= 40, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"

seq_t a(idx_t N) {
    if (N == 0) return 0;          /* undefined, guard for safety */

    idx_t m = N - 1;               /* zero-based index in the sequence */

    /* Compute d such that (d+1)(d+2)/2 > m  (triangular numbers) */
    double t = sqrt((double)(8.0 * (double)m + 1.0));
    idx_t d = (idx_t)((t - 1.0) / 2.0);

    /* Correct d for possible rounding errors */
    while (((unsigned __int128)(d + 1) * (d + 2)) / 2 <= (unsigned __int128)m) d++;
    while (((unsigned __int128)d * (d + 1)) / 2 > (unsigned __int128)m) d--;

    idx_t total_prev = d * (d + 1) / 2;
    idx_t offset   = m - total_prev;
    idx_t i        = offset + 1;          /* the n in T(n,k) */
    idx_t j        = d - offset;          /* the k in T(n,k) */

    seq_t sum = 0;

    /* Sum over all positive divisors of i */
    idx_t sq = (idx_t)sqrt((double)i);
    for (idx_t d1 = 1; d1 <= sq; ++d1) {
        if (i % d1 == 0) {
            sum += binomial(d1 + j - 1, j);
            idx_t d2 = i / d1;
            if (d2 != d1) {
                sum += binomial(d2 + j - 1, j);
            }
        }
    }

    return sum;
}
