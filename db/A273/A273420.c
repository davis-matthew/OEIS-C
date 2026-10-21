/*
 * A273420  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 705", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n + 9 for even n > 2, 8*n + 7 for odd n > 2; a(0) = 3,
 *            a(1) = 17, a(2) = 20
 * Range:     u8 n <= 31, u16 n <= 127, u32 n <= 127, u64 n <= 127,
 *            u128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three special values. */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)17;
    if (n == 2) return (seq_t)20;

    /* n > 2: use the closed‑form expression. */
    if ((n & 1) == 0)         /* even */
        return (seq_t)8 * (seq_t)n + (seq_t)9;
    else                       /* odd */
        return (seq_t)8 * (seq_t)n + (seq_t)7;
}
