/*
 * A341736  a(n) is the label of the square of the n-th element in the
 *          semigroup S = {(0,0), (i,j): i >= j >= 1}.
 * https://oeis.org/A341736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = label of the n-th element of the semigroup S squared,
 *            equivalently a(n) = A341317(n,n) = A341318(n,n)
 * Range:     u8 n <= 10, u16 n <= 189, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* Helper function to compute floor(sqrt(x)) for 64‑bit values. */
static inline idx_t isqrt_u64(idx_t x) {
    /* Using sqrt with a few corrections to avoid rounding errors. */
    idx_t r = (idx_t)floor(sqrt((long double)x));
    while ((r + 1) * (r + 1) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* a(0) = 0 */

    /* Determine k such that B_k = 1 + k(k+1)/2 <= n < B_{k+1}. */
    idx_t t = n - 1;                                /* t = n-1 */
    idx_t s = isqrt_u64(8 * t + 1);
    idx_t k = (s - 1) / 2;                          /* integer k >= 0 */

    /* Compute B_k = 1 + k(k+1)/2 and offset d = n - B_k. */
    seq_t k_s = (seq_t)k;
    seq_t Bk   = (seq_t)1 + (k_s * (k_s + 1) / 2);
    seq_t d    = (seq_t)n - Bk;                     /* 0 <= d <= k */

    /* a(B_k) = (k^4 + 4k^3 + 5k^2 + 2k + 2) / 2 */
    seq_t k2  = k_s * k_s;
    seq_t k3  = k2 * k_s;
    seq_t k4  = k3 * k_s;
    seq_t Ak  = (k4 + 4 * k3 + 5 * k2 + 2 * k_s + 2) / 2;

    if (d == 0) return Ak;                          /* exact big‑jump position */

    /* Small increments add d^2 + 2d (sum of odd numbers 3,5,…). */
    return Ak + d * d + 2 * d;
}
