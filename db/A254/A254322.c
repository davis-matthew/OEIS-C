/*
 * A254322  Expansion of e.g.f.: (1-11*x)^(-10/11).
 * https://oeis.org/A254322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n-1) * a(n-1); a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t result = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        /* factor (11*k - 1) is always positive for k >= 1 */
        result *= (seq_t)(11 * k - 1);
    }
    return result;
}
