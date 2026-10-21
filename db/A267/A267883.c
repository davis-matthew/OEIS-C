/*
 * A267883  Number of OFF (white) cells in the n-th iteration of the "Rule
 *          233" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A267883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0; a(n)=max(0,4-n) for n>=1 (so a(1)=3, a(2)=2, a(3)=1,
 *            a(n)=0 for n>=4)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n >= 4) return 0;
    return (seq_t)(4 - (seq_t)n);
}
