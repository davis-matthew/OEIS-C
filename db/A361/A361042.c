/*
 * A361042  Triangle read by rows: T(n, k) = Sum_{j=0..n} j! * binomial(n - j,
 *          n - k).
 * https://oeis.org/A361042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the unique integer with * r*(r+1)/2 <=
 *            n < (r+1)*(r+2)/2 and k = n - r*(r+1)/2. * T(r,k) = sum_{j=0..k}
 *            j! * binomial(r-j, r-k)
 * Range:     u8 n <= 26, u16 n <= 53, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((unsigned __int128)r * (r + 1) / 2 <= n) {
        r++;
    }
    r--; /* now r satisfies the inequality */

    /* Compute column index k in that row */
    idx_t first = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - first;

    /* Compute T(r,k) = sum_{j=0..k} j! * binomial(r-j, r-k) */
    seq_t sum = 0;
    seq_t fact = 1; /* j! for current j */

    for (idx_t j = 0; j <= k; ++j) {
        seq_t bin = binomial(r - j, r - k);
        sum += fact * bin;
        fact = fact * (j + 1);
    }

    return sum;
}
