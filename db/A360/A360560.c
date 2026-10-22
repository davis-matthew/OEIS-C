/*
 * A360560  Triangle read by rows. T(n, k) = (1/2) * C(n, k) * C(3*n - 1, n)
 *          for n > 0 and T(0, 0) = 1.
 * https://oeis.org/A360560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (1/2) * binomial(r,k) * binomial(3*r - 1, r) where r =
 *            row index (starting from 0) and k is the position within that
 *            row, obtained from the flattened triangle order
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;   /* row 0 term */

    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t low = 0, high = 1;
    while ((high * (high + 1) / 2) <= n) {
        low = high;
        high <<= 1;          /* double the search range */
    }
    while (high - low > 1) {
        idx_t mid = (low + high) >> 1;
        if ((mid * (mid + 1) / 2) <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t r = low;
    idx_t k = n - r * (r + 1) / 2;  /* offset within the row */

    seq_t bin_rk = binomial(r, k);

    /* Compute binomial(3*r - 1, r).  Use 128‑bit math to avoid overflow
       when 3*r might exceed the width of idx_t. */
    unsigned __int128 tmp = (unsigned __int128)3 * r - 1;
    idx_t m = (idx_t)tmp;   /* safe when 3*r-1 fits in idx_t */
    seq_t bin_3r = binomial(m, r);

    return (bin_rk * bin_3r) / 2;
}
