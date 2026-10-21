/*
 * A269878  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 43", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A269878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = if n even: (4*n^3 + 6*n^2 + 2*n + 6) / 6 if n odd :
 *            (4*n^3 + 15*n^2 + 14*n + 3) / 6
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t sq = sn * sn;
    seq_t cube = sq * sn;

    if ((n & 1) == 0) /* even n */
        return (4 * cube + 6 * sq + 2 * sn + 6) / 6;
    else                /* odd n */
        return (4 * cube + 15 * sq + 14 * sn + 3) / 6;
}
