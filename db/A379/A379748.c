/*
 * A379748  a(n) is the number of ways to arrange any number of unit square
 *          cells into an i X j rectangle which contains exactly n square
 *          subarrays of all sizes.
 * https://oeis.org/A379748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (m,k) with m≥k≥1 such that
 *            k(k+1)(3m−k+1)/6 = n. This counts distinct i×j rectangles (with
 *            i≥j) that contain exactly n square subarrays
 * Range:     u8 n <= 42, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h> /* for bool, not strictly needed but safe */

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t N = (seq_t)n;          /* cast to seq_t for arithmetic */
    seq_t t = 6 * N;             /* 6*n, used repeatedly */

    for (idx_t k = 1; ; ++k) {
        seq_t kk = (seq_t)k;

        /* Check if the minimal number of squares with m = k is already > n. */
        seq_t val = kk * (kk + 1) * (2 * kk + 1) / 6;
        if (val > N) break;     /* k exceeded maximum possible value */

        seq_t denom = kk * (kk + 1);
        if (t % denom != 0) continue;   /* 6n must be divisible by k(k+1) */

        seq_t num = t / denom;           /* 3m - k + 1 */
        seq_t rhs = num + kk - 1;        /* numerator for m */
        if (rhs % 3 != 0) continue;      /* m must be integer */

        seq_t m = rhs / 3;
        if (m < kk) continue;            /* ensure m ≥ k */

        ++count;                          /* valid rectangle */
    }

    return count;
}
