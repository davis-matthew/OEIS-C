/*
 * A267881  Number of ON (black) cells in the n-th iteration of the "Rule 233"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + 1 for n ≥ 5; a(0) = 1, a(1) = 0, a(2) = 3, a(3) =
 *            6, a(4) = 9, a(5) = 11
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)9;
    if (n == 5) return (seq_t)11;
    return (seq_t)n * (seq_t)2 + (seq_t)1;
}
