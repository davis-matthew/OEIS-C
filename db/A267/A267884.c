/*
 * A267884  Total number of OFF (white) cells after n iterations of the "Rule
 *          233" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A267884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n=0; 3 if n=1; 5 if n=2; 6 if n>2
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)5;
    return (seq_t)6;
}
