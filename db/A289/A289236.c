/*
 * A289236  Square array a(p,q) read by antidiagonals: a(p,q) = the number of
 *          line segments that constitute the trajectory of a billiard ball on
 *          a pool table with dimensions p X q, before the ball reaches a
 *          corner.
 * https://oeis.org/A289236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   gcd, sum_mid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A289236: Number of segments of a billiard path on a p×q table.  
   a(n) = (p+q)/gcd(p,q) - 1, where (p,q) is the pair located at the
   n-th position when the infinite table is read by antidiagonals. */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/sum_mid.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* position 0 is outside the offset, return 0 */

    /* Find s such that s*(s-1)/2 >= n > (s-2)*(s-1)/2 */
    idx_t low = 2, high = 2;
    while (sum_mid(high) < (unsigned __int128)n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (sum_mid(mid) >= (unsigned __int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;

    /* Compute the offset within the current diagonal */
    idx_t prev = (s - 1u) * (s - 2u) / 2u;
    idx_t offset = n - prev;  /* 1-based within diagonal of sum s */

    /* Map to (p,q) coordinates */
    idx_t p = offset;
    idx_t q = s - offset;

    /* Compute value: (p+q)/gcd(p,q) - 1 */
    seq_t g = gcd((seq_t)p, (seq_t)q);
    seq_t ans = (seq_t)s / g - 1;
    return ans;
}
