/*
 * A338198  Triangle read by rows, T(n,k) = ((k+1)*2^(n-k)-(k-2)*(-1)^(n-k))/3
 *          for 0 <= k <= n.
 * https://oeis.org/A338198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A338198 – triangle read by rows.
 *
 * a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2) is the row index
 * and k = n - r*(r+1)/2 is the column index within that row.
 *
 * The triangle entry is
 *   T(r,k) = ((k+1)*2^(r-k) - (k-2)*(-1)^(r-k)) / 3
 *   for 0 ≤ k ≤ r.
 *
 * The function uses the provided pow2 helper to compute powers of two.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0;
    idx_t high = 1;
    /* Expand high until the triangular number exceeds n */
    while ( (( __uint128_t)high * (high + 1) ) / 2 <= n )
        high <<= 1;

    /* Binary search for the largest r with triangular number <= n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ( (( __uint128_t)mid * (mid + 1) ) / 2 <= n )
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low - 1;

    /* Compute k = n - r*(r+1)/2 */
    idx_t sum = (idx_t)( (( __uint128_t)r * (r + 1) ) / 2 );
    idx_t k = n - sum;

    /* Compute ((k+1)*2^(r-k) - (k-2)*(-1)^(r-k)) / 3 */
    seq_t pow_two = pow2(r - k);
    seq_t term1 = (seq_t)(k + 1) * pow_two;

    /* (k-2)*(-1)^(r-k) as signed value */
    int64_t kk = (int64_t)k;
    int64_t delta = (kk - 2) * ( ((r - k) & 1) ? -1 : 1 );

    seq_t numerator;
    if (delta > 0) {
        numerator = term1 - (seq_t)delta;
    } else {
        numerator = term1 + (seq_t)(-delta);
    }

    return numerator / 3;
}
