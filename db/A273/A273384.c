/*
 * A273384  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 659", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1+2*n)^2 for n > 2 Initial values: a(0) = 1, a(1) = 5,
 *            a(2) = 17 See OEIS A273384
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 5;
    if (n == 2) return 17;
    seq_t t = 1 + (seq_t)2 * n;
    return t * t;
}
