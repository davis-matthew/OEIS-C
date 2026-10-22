/*
 * A369817  The fifth term of the greedy B_n set of natural numbers.
 * https://oeis.org/A369817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n + 3)/2) * n^2 + floor((3*n + 2)/2)
 * Range:     u8 n <= 6, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t part1 = ((N + (seq_t)3) / (seq_t)2) * (N * N);
    seq_t part2 = ((seq_t)3 * N + (seq_t)2) / (seq_t)2;
    return part1 + part2;
}
