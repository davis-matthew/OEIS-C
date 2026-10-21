/*
 * A245827  Szeged index of the grid graph P_3 X P_n.
 * https://oeis.org/A245827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(17*n^2 - 9)/2
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 644, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t sq = ns * ns;
    seq_t term = (seq_t)17 * sq;
    term -= (seq_t)9;
    seq_t res = ns * term;
    res /= 2;
    return res;
}
