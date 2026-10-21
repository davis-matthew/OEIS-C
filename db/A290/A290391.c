/*
 * A290391  Number of 5-cycles in the n-triangular honeycomb obtuse knight
 *          graph.
 * https://oeis.org/A290391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n < 7; for n >= 7: a(n) = 6*(292 - 90*n + 7*n^2)
 * Range:     u8 n <= 8, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 7)
        return (seq_t)0;
    seq_t nn = (seq_t)n;
    seq_t term = 6 * ((seq_t)292 - (seq_t)90 * nn + (seq_t)7 * nn * nn);
    return term;
}
