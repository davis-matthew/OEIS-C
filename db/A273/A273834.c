/*
 * A273834  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 961", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=3, a(1)=17, a(2)=24, a(n)=8+(-1)^n+8*n for n>2 (which
 *            equals 9+8*n for even n>2 and 7+8*n for odd n>2)
 * Range:     u8 n <= 31, u16 n <= 127, u32 n <= 127, u64 n <= 127,
 *            u128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)17;
    if (n == 2) return (seq_t)24;

    /* For n > 2 */
    return ((seq_t)n * 8) + (seq_t)((n & 1ULL) ? 7 : 9);
}
