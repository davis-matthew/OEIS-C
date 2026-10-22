/*
 * A320539  (1/2) * number of ways to select 3 distinct collinear points from
 *          a rectangle of grid points with side lengths j and k, written as
 *          triangle T(j,k), j<=k.
 * https://oeis.org/A320539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (1/2) * number of 3‑point collinear subsets in a
 *            (j+1)×(k+1) grid where n indexes the pairs (j,k) with j ≤ k by
 *            antidiagonal order (j = n – (k‑1)·k/2, k = smallest integer with
 *            k(k+1)/2 ≥ n)
 * Range:     u8 n <= 14, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static inline seq_t binom3(idx_t n)
{
    if (n < 3) return 0;
    return ((seq_t)n * (n - 1) * (n - 2)) / 6;
}

seq_t a(idx_t n)
{
    /* find k such that k(k+1)/2 ≥ n */
    idx_t low = 1, high = 1;
    while (high * (high + 1) / 2 < n) {
        high *= 2;
    }
    while (low < high) {
        idx_t mid = (low + high) / 2;
        if (mid * (mid + 1) / 2 < n) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    idx_t k = low;                       /* rectangle width */
    idx_t j = n - (k - 1) * k / 2;        /* rectangle height, j ≤ k */

    /* grid sizes in points */
    seq_t J = (seq_t)(j + 1);
    seq_t K = (seq_t)(k + 1);

    seq_t total = 0;

    /* horizontal and vertical lines */
    total += J * binom3(k + 1);   /* each of the j+1 rows contributes C(k+1,3) */
    total += K * binom3(j + 1);   /* each of the k+1 columns contributes C(j+1,3) */

    /* primitive slopes */
    for (idx_t dx = 1; dx <= j; ++dx) {
        for (idx_t dy = 1; dy <= k; ++dy) {
            if (gcd_idx(dx, dy) != 1) continue;

            /* positive slope (dx,dy) */
            /* group 1: x < dx */
            for (idx_t x = 0; x < dx; ++x) {
                for (idx_t y = 0; y <= k; ++y) {
                    idx_t tmax1 = (j - x) / dx;
                    idx_t tmax2 = (k - y) / dy;
                    idx_t tmax = tmax1 < tmax2 ? tmax1 : tmax2;
                    seq_t len = (seq_t)(tmax + 1);
                    if (tmax + 1 >= 3) total += binom3(tmax + 1);
                }
            }
            /* group 2: x ≥ dx, y < dy */
            for (idx_t x = dx; x <= j; ++x) {
                for (idx_t y = 0; y < dy; ++y) {
                    idx_t tmax1 = (j - x) / dx;
                    idx_t tmax2 = (k - y) / dy;
                    idx_t tmax = tmax1 < tmax2 ? tmax1 : tmax2;
                    if (tmax + 1 >= 3) total += binom3(tmax + 1);
                }
            }

            /* negative slope (-dy,dx) */
            /* group 1: x < dx */
            for (idx_t x = 0; x < dx; ++x) {
                for (idx_t y = 0; y <= k; ++y) {
                    idx_t tmax1 = (j - x) / dx;
                    idx_t tmax2 = y / dy;
                    idx_t tmax = tmax1 < tmax2 ? tmax1 : tmax2;
                    if (tmax + 1 >= 3) total += binom3(tmax + 1);
                }
            }
            /* group 2: x ≥ dx, y + dy > k  →  y ≥ k - dy + 1 */
            for (idx_t x = dx; x <= j; ++x) {
                for (idx_t y = k - dy + 1; y <= k; ++y) {
                    idx_t tmax1 = (j - x) / dx;
                    idx_t tmax2 = y / dy;
                    idx_t tmax = tmax1 < tmax2 ? tmax1 : tmax2;
                    if (tmax + 1 >= 3) total += binom3(tmax + 1);
                }
            }
        }
    }

    return total / 2;
}
