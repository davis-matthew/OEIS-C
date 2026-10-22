/*
 * A375550  Triangle read by rows: T(m, n, k) = binomial(n + 1, n -
 *          k)*hypergeom([m, k - n], [k + 2], -1) for m = 4.
 * https://oeis.org/A375550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(3,r,k) where r and k are obtained from n by r*(r+1)/2
 *            <= n < (r+1)*(r+2)/2 and k = n - r*(r+1)/2. T(3,r,k) =
 *            Σ_{j=0}^{r-k} binomial(3+j,3) * binomial(r+1, r-(j+k))
 * Range:     u8 n <= 9, u16 n <= 54, u32 n <= 275, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* --- helper: integer square root for 64‑bit unsigned value ---------------- */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, mid, best = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        /* test mid² ≤ n without overflow */
        if (mid == 0 || mid <= n / mid) {
            best = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return best;
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    idx_t start = 0;
    while (start <= n) {
        r++;
        start = r * (r + 1) / 2;
    }
    r--;                /* r now satisfies r*(r+1)/2 <= n */
    start = r * (r + 1) / 2;
    idx_t k = n - start;/* column index within the row */

    /* Sum the terms of T(3,r,k) */
    seq_t result = 0;
    seq_t term1 = 1;    /* binomial(3,3) */

    idx_t max_j = r - k;
    for (idx_t j = 0; j <= max_j; ++j) {
        seq_t term2 = binomial(r + 1, r - (j + k));
        result += term1 * term2;

        /* Update term1 for the next j (recurrence for binomial(3+j,3)) */
        if (j < max_j) {
            term1 = (term1 * (seq_t)(4 + j)) / (seq_t)(j + 1);
        }
    }
    return result;
}
