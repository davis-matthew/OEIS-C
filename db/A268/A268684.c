/*
 * A268684  a(n) = n*(n + 1)*(4*n - 1)/3.
 * https://oeis.org/A268684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(4*n - 1)/3
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term = nn * (nn + 1);
    term *= ((seq_t)4 * nn - 1);
    return term / 3;
}
