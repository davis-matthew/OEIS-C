/*
 * A269174  Formula for Wolfram's Rule 124 cellular automaton: a(n) = (n OR
 *          2n) AND ((n XOR 2n) OR (n XOR 4n)).
 * https://oeis.org/A269174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n OR 2n) AND ((n XOR 2n) OR (n XOR 4n))
 * Range:     u8 n <= 127, u16 n <= 9965, u32 n <= 9965, u64 n <= 9965,
 *            u128 n <= 9965, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t two = nn << 1;
    seq_t four = nn << 2;
    seq_t part1 = nn | two;
    seq_t part2 = (nn ^ two) | (nn ^ four);
    return part1 & part2;
}
