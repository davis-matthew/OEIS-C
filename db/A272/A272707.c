/*
 * A272707  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 515", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A272707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 + n^2 + 3n + 1 if n is even; * a(n) = n^3 + n^2 + 2n
 *            + 2 if n is odd
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* convert index to seq_t */
    seq_t n2 = sn * sn;           /* n^2 */
    seq_t n3 = n2 * sn;           /* n^3 */
    if ((sn & (seq_t)1) == 0) {   /* n even */
        return n3 + n2 + 3 * sn + 1;
    } else {                      /* n odd */
        return n3 + n2 + 2 * sn + 2;
    }
}
