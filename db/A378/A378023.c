/*
 * A378023  a(n) = (2*n^3 - 6*n^2 + 19*n - 9)*n/6.
 * https://oeis.org/A378023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 - 6*n^2 + 19*n - 9)*n/6
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t  = (nn * nn * nn) * 2 - (nn * nn) * 6 + nn * 19 - 9;
    return t * nn / 6;
}
