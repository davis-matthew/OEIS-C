/*
 * A273743  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 901", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 0, a(1) = 8, a(n) = (1 + 2n)^2 for n > 1
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)8;
    /* use seq_t to avoid overflow when n is large */
    seq_t tn = (seq_t)n;
    seq_t two_n_plus1 = tn * (seq_t)2 + (seq_t)1;
    return two_n_plus1 * two_n_plus1;
}
