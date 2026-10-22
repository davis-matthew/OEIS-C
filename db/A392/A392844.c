/*
 * A392844  Stellated octahemioctahedral numbers.
 * https://oeis.org/A392844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^3 - 12*n + 5
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t cube = nn * nn * nn;
    seq_t res = 8 * cube - 12 * nn + 5;
    return res;
}
