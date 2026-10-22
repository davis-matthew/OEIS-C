/*
 * A318397  Triangle read by rows: T(n,k) = binomial(n,k)^2 *
 *          binomial(2*(n-k), n-k).
 * https://oeis.org/A318397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(r,k)^2 * binomial(2*(r-k), r-k) where r =
 *            floor((sqrt(1+8*n)-1)/2) and k = n - r*(r+1)/2. a(n) is the
 *            triangle T(r,k) read row by row
 * Range:     u8 n <= 9, u16 n <= 29, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include <math.h>

/* Compute OEIS sequence A318397 */
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r;
    /* Initial estimate using double sqrt; later adjust if necessary */
    {
        double v = sqrt((double)(1ULL + 8ULL * (unsigned long long)n));
        r = (idx_t)((v - 1.0) / 2.0);
    }
    /* Correct possible rounding errors */
    while (r * (r + 1) / 2 > n) {
        r--;
    }
    while ((r + 1) * (r + 2) / 2 <= n) {
        r++;
    }

    /* Compute k = n - r*(r+1)/2 */
    idx_t prev = r * (r + 1) / 2;
    idx_t k = n - prev;

    /* T(r,k) = binomial(r,k)^2 * binomial(2*(r-k), r-k) */
    seq_t bin_rk = binomial(r, k);
    seq_t bin_square = bin_rk * bin_rk;
    idx_t m = 2 * (r - k);
    seq_t bin_m   = binomial(m, r - k);
    return bin_square * bin_m;
}
