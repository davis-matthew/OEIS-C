/*
 * A376057  a(n) is the denominator of the sum S(n) defined in A376056.
 * https://oeis.org/A376057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, frac
 * Formula:   a(0) = 1; a(n+1) = (2*n+1)*a(n)^2 + a(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1; /* a(0) */
    for (idx_t i = 0; i < n; ++i) {
        seq_t term = (seq_t)(2 * i + 1) * res * res + res;
        res = term;
    }
    return res;
}
