/*
 * A385178  Triangle T(n,k) read by rows in which the n-th diagonal lists the
 *          n-th differences of A001047, 0 <= k <= n.
 * https://oeis.org/A385178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,k) = 2^(n-k)*3^k - 2^k, read by rows. a(n) = T(r,k) where r
 *            = floor((sqrt(8n+1)-1)/2), k = n - r(r+1)/2
 * Range:     u8 n <= 24, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t low = 0;
    idx_t high = 1;
    /* Grow high until it exceeds n */
    while ((unsigned __int128)high * (high + 1) / 2 <= (unsigned __int128)n)
        high <<= 1;
    /* Binary search for largest r with r(r+1)/2 <= n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= (unsigned __int128)n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low - 1;

    /* Column index k */
    idx_t offset = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - offset;

    /* Compute T(r,k) = 2^(r-k)*3^k - 2^k */
    seq_t term = pow2(r - k) * pow3(k) - pow2(k);
    return term;
}
