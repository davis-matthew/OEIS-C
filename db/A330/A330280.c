/*
 * A330280  Least number of 2 X 2 rotations required to solve an n X n Simon
 *          Tatham's Twiddle puzzle.
 * https://oeis.org/A330280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = (n-1)*(3*n-4)
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t term = (n_seq - 1) * (3 * n_seq - 4);
    return term;
}
