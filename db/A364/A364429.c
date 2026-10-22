/*
 * A364429  a(0) = 1, a(n) = (2*n^5 + 20*n^3 + 23*n) * 2/15 for n>=1.
 * https://oeis.org/A364429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^5 + 20*n^3 + 23*n) * 2/15
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;            /* n² */
    seq_t n3 = n2 * nn;            /* n³ */
    seq_t n5 = n3 * n2;            /* n⁵ */
    seq_t term = (seq_t)2 * n5 + (seq_t)20 * n3 + (seq_t)23 * nn;
    term = term * (seq_t)2;
    term = term / (seq_t)15;       /* exact division */
    return term;
}
