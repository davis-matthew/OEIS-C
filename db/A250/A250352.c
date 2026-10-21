/*
 * A250352  Number of length 3 arrays x(i), i=1..3 with x(i) in i..i+n and no
 *          value appearing more than 2 times.
 * https://oeis.org/A250352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 + 3*n^2 + 2*n + 2 (OEIS A250352)
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t result = n3 + ((seq_t)3 * n2) + ((seq_t)2 * nn) + (seq_t)2;
    return result;
}
