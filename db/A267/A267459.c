/*
 * A267459  Total number of ON (black) cells after n iterations of the "Rule
 *          133" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A267459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^2 - 4*n + (-1)^n + 11)/4 for n>0; a(0) = 1
 * Range:     u8 n <= 12, u16 n <= 233, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;                    /* n^2 */
    seq_t add = (n & 1) ? (seq_t)10 : (seq_t)12;   /* 10 if n odd, 12 if n even */
    seq_t numerator = (seq_t)2 * N2 - (seq_t)4 * N + add;

    return numerator / (seq_t)4;
}
