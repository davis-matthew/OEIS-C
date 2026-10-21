/*
 * A277046  Triangle read by rows: T(n,k) = 2^n - n + k - 1 for n >= 1, with 1
 *          <= k <= 2n-1.
 * https://oeis.org/A277046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 2^row - row + col - 1, where row = ⌊√(n-1)⌋+1 and col = n
 *            - (row-1)^2; the triangle is read by rows
 * Range:     u8 n <= 57, u16 n <= 241, u32 n <= 993, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

/* integer square root for unsigned 64-bit integers */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 m2 = (__int128)mid * mid;
        if (m2 <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* compute row index (1-based) */
    idx_t row = isqrt_u64(n - 1) + 1;

    /* compute column index within the row (1-based) */
    idx_t prev_sq = (idx_t)((__int128)(row - 1) * (row - 1));
    idx_t col = n - prev_sq;

    /* calculate the term: 2^row - row + col - 1 */
    seq_t pow2 = seq_pow((seq_t)2, row);
    seq_t result = pow2 - (seq_t)row + (seq_t)(col - 1);

    return result;
}
