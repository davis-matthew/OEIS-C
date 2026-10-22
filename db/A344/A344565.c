/*
 * A344565  Triangle read by rows, for 0 <= k <= n: T(n, k) = binomial(n, k) *
 *          binomial(binomial(n + 3, 2), 2).
 * https://oeis.org/A344565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n + 1) - 1) / 2) k = n -
 *            r*(r+1)/2 and T(r,k) = binomial(r,k) * binomial((r+3 choose 2),
 *            2)
 * Range:     u8 n <= 6, u16 n <= 38, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Integer square root for 64‑bit unsigned integers.
   Uses binary search with 128‑bit multiplication to avoid overflow. */
static inline idx_t isqrt_u64(idx_t n) {
    idx_t left = 0, right = ((idx_t)1 << 32); /* sqrt(2^64-1) < 2^32 */
    while (left < right) {
        idx_t mid = left + (right - left + 1) / 2;
        if ((unsigned __int128)mid * mid <= n)
            left = mid;
        else
            right = mid - 1;
    }
    return left;
}

seq_t a(idx_t n) {
    /* Compute the row r that contains the n‑th flattened element. */
    idx_t sqrt_term = isqrt_u64((idx_t)(((__int128)8 * n) + 1));
    idx_t r = (sqrt_term - 1) / 2;

    /* Index k within that row. */
    idx_t row_start = r * (r + 1) / 2;
    idx_t k = n - row_start;

    /* Compute the triangle entry T(r,k). */
    seq_t bin_rk = binomial(r, k);                   /* binomial(r,k)   */
    idx_t m = (r + 3) * (r + 2) / 2;                 /* (r+3 choose 2) */
    seq_t bin_m2 = binomial(m, 2);                   /* binomial(m,2)   */

    return bin_rk * bin_m2;
}
