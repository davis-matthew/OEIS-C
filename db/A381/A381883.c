/*
 * A381883  Triangle read by rows: T(n, k) = binomial(2*n - 1, k).
 * https://oeis.org/A381883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(2*r-1 , k) where r is the largest integer with
 *            r*(r+1)/2 <= n and k = n - r*(r+1)/2
 * Range:     u8 n <= 18, u16 n <= 50, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 v = (unsigned __int128)mid * (mid + 1) / 2;
        if (v <= n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    idx_t k = n - (r * (r + 1) / 2);

    /* r == 0 corresponds to row 0 with a single element 1 */
    if (r == 0) return (seq_t)1;

    return binomial((idx_t)(2 * r - 1), (idx_t)k);
}
