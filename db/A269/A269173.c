/*
 * A269173  Formula for Wolfram's Rule 126 cellular automaton: a(n) = (n XOR
 *          2n) OR (n XOR 4n).
 * https://oeis.org/A269173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n XOR 2n) OR (n XOR 4n)
 * Range:     u8 n <= 63, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t y1 = x ^ ((seq_t)n << 1);
    seq_t y2 = x ^ ((seq_t)n << 2);
    return y1 | y2;
}
