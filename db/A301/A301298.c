/*
 * A301298  Expansion of (1 + 4*x + 4*x^2 + 4*x^3 + x^4)/((1 - x)*(1 - x^3)).
 * https://oeis.org/A301298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n - floor((n + 1)/3), a(0) = 1
 * Range:     u8 n <= 54, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    seq_t m = (seq_t)n;
    return 5 * m - (m + 1) / 3;
}
