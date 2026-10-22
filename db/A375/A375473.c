/*
 * A375473  a(n) is the area of the largest rectangle with integer sides that
 *          can be inscribed under the parabola y = -x^2 + n and on or above
 *          the x-axis.
 * https://oeis.org/A375473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*floor(1/2 + sqrt(n/3 - 1/12))*(-(floor(1/2 + sqrt(n/3 -
 *            1/12)))^2 + n)
 * Range:     u8 n <= 47, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    /* Compute the integer k = floor(1/2 + sqrt(n/3 - 1/12)) */
    idx_t m = 4 * n - 1;          /* m = 4n - 1 */
    idx_t k = 0;
    while (1) {
        idx_t kp1 = k + 1;
        /* f(k+1) = 12*(k+1)^2 - 12*(k+1) + 3 */
        idx_t f = 12 * kp1 * kp1 - 12 * kp1 + 3;
        if (f > m) break;
        k = kp1;
    }
    seq_t nn = (seq_t)n;
    seq_t kk = (seq_t)k;
    seq_t kk_sq = kk * kk;
    return 2 * kk * (nn - kk_sq);
}
