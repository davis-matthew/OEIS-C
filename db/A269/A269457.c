/*
 * A269457  a(n) = 5*(n + 1)*(n + 4)/2.
 * https://oeis.org/A269457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*(n + 1)*(n + 4)/2
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = ((seq_t)n + 1) * ((seq_t)n + 4);
    t /= 2;
    return t * (seq_t)5;
}
