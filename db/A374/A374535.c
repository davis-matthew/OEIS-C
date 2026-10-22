/*
 * A374535  Expansion of (x/(8 * (1-x))) * d/dx(theta_3(x)^4).
 * https://oeis.org/A374535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{d=1..n, 4∤ d} d² * floor(n/d) * (floor(n/d)+1) / 2
 * Range:     u8 n <= 8, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t d = 1; d <= n; ++d) {
        if (d % 4 == 0) continue;
        idx_t m = n / d;
        seq_t t = (seq_t)m * (m + 1) / 2;
        seq_t dd = (seq_t)d * d;
        total += dd * t;
    }
    return total;
}
