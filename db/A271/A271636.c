/*
 * A271636  a(n) = 4*n*(4*n^2 + 3).
 * https://oeis.org/A271636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n*(4*n^2 + 3)
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 645, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return (seq_t)4 * tn * ((seq_t)4 * tn * tn + (seq_t)3);
}
