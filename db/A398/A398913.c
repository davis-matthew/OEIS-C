/*
 * A398913  Table read by rows: the number of functions f: [n] -> [n] where
 *          the longest initial run of strictly increasing values has length
 *          k.
 * https://oeis.org/A398913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r and k are obtained by row‑major *
 *            traversal of the triangle T(n,k): * T(n,k) = k * binomial(n+1,
 *            k+1) * n^(n - k - 1) (k < n) * T(n,n) = 1
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 55, u64 n <= 153,
 *            u128 n <= 406, big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Handle the offset (n==0) */
    if (n == 0) return (seq_t)1;

    /* Find row r such that cumulative elements up to r-1 <= n < cumulative up to r */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) {
        r++;
    }

    /* Number of elements before this row */
    idx_t prev_cum = r * (r + 1) / 2;
    /* Column within the row */
    idx_t k = n - prev_cum;

    /* Row 0 has only one entry which is 1 (handled by n==0 above) */
    if (k == r) {
        /* Last element in the row, always 1 */
        return (seq_t)1;
    }

    /* Compute T(r,k) for k < r */
    seq_t bin = binomial(r + 1, k + 1);
    seq_t exp = (seq_t)(r - k - 1);
    seq_t pow = big_pow((seq_t)r, exp);

    seq_t result = (seq_t)k;
    result = result * bin;
    result = result * pow;

    return result;
}
