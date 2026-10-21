/*
 * A264774  Triangle T(n,k) = binomial(5*n - 4*k, 4*n - 3*k), 0 <= k <= n.
 * https://oeis.org/A264774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) enumerates the triangle T(r,k) = binomial(5*r - 4*k, r - k)
 *            (row‑major with offset 0)
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 44, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Integer square root of a 64‑bit unsigned integer */
static idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0;
    idx_t high = (idx_t)(1ULL << 32);          /* high enough for all 64‑bit values */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid > 0 && mid > x / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t s = isqrt_u64(1ULL + 8ULL * n);
    idx_t r = (s - 1) / 2;

    /* Column index k within the row */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* Return the binomial coefficient */
    return binomial(5 * r - 4 * k, r - k);
}
