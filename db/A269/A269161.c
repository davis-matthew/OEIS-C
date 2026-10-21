/*
 * A269161  Formula for Wolfram's Rule 86 cellular automaton: a(n) = 4n XOR
 *          (2n OR n).
 * https://oeis.org/A269161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4n XOR (2n OR n)
 * Range:     u8 n <= 63, u16 n <= 9890, u32 n <= 9890, u64 n <= 9890,
 *            u128 n <= 9890, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t mul4 = N * 4UL;
    seq_t mul2_or = (N * 2UL) | N;
    return mul4 ^ mul2_or;
}
