/*
 * A271828  a(n) = 4*n^3 - 18*n^2 + 27*n - 12.
 * https://oeis.org/A271828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^3 - 18*n^2 + 27*n - 12
 * Range:     u8 n <= 5, u16 n <= 26, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t fn = (seq_t)n;
    return 4*fn*fn*fn - 18*fn*fn + 27*fn - 12;
}
