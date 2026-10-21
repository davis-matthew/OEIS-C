/*
 * A260113  Maximum number of queens on an n X n chessboard such that no queen
 *          attacks more than one other queen.
 * https://oeis.org/A260113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n for n ≤ 5 * a(n) = ⌊4*n/3⌋ for n > 5
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 5)
        return (seq_t)n;

    return ((seq_t)n * (seq_t)4) / (seq_t)3;
}
