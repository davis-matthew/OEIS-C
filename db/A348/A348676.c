/*
 * A348676  Triangle read by rows, T(n, k) = 2^(n - HammingWeight(k)), for 0
 *          <= k <= n.
 * https://oeis.org/A348676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^(r - popcount(k)), where r = floor((sqrt(8n+1)-1)/2) is
 *            the row index and k = n - r*(r+1)/2 is the column index
 *            (0‑based)
 * Range:     u8 n <= 35, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Locate the row r that contains the n‑th element. */
    idx_t low = 0, high = n + 1;          /* high is exclusive */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t s = (idx_t)(((__int128)mid * (mid + 1)) / 2);
        if (s <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t r = low;

    /* Compute the column index k within that row. */
    idx_t prev_total = (idx_t)(((__int128)r * (r + 1)) / 2);
    idx_t k = n - prev_total;

    /* Number of 1‑bits in k. */
    unsigned int pop = __builtin_popcountll((uint64_t)k);

    /* Exponent for the power of two. */
    idx_t exp = r - pop;

    /* Return 2^exp. */
    return pow2(exp);
}
