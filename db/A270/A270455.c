/*
 * A270455  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 163", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 + 9*n^2 + 8*n + 6) / 6 for n even a(n) = (4*n^3 +
 *            15*n^2 + 14*n + 3) / 6 for n odd where n is the step index
 *            (a(0)=1, a(1)=6, …)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t s   = (seq_t)n;       /* cast to arithmetic type */
    seq_t n2  = s * s;          /* n^2 */
    seq_t n3  = n2 * s;         /* n^3 */

    if ((n & 1) == 0) {        /* n even */
        return ( (seq_t)4 * n3
                 + (seq_t)9 * n2
                 + (seq_t)8 * s
                 + 6 ) / 6;
    } else {                   /* n odd */
        return ( (seq_t)4 * n3
                 + (seq_t)15 * n2
                 + (seq_t)14 * s
                 + 3 ) / 6;
    }
}
