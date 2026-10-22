/*
 * A374443  Triangle read by rows: T(n, k) = rad(gcd(n, k)) if n, k > 0, T(0,
 *          0) = 1, where rad = A007947 and gcd = A109004.
 * https://oeis.org/A374443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = rad(gcd(r, k)) where (r, k) are the row and column
 *            indices of n when the triangle is read row‑wise. For n=0 the
 *            only entry is 1. The mapping satisfies r = floor((-1 + sqrt(1 +
 *            8*n)) / 2), k = n - r*(r+1)/2
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   gcd, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/rad.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2
       using a binary search; 128‑bit arithmetic avoids overflow. */
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    /* Column index k (0‑based) within the row. */
    idx_t k = n - (idx_t)((unsigned __int128)r * (r + 1) / 2);

    /* Compute rad(gcd(r, k)). Both r and k fit in idx_t, so they fit in seq_t. */
    seq_t g = gcd((seq_t)r, (seq_t)k);
    return rad((idx_t)g);
}
