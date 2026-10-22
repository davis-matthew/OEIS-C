/*
 * A318778  Number of different positions that an elementary sphinx can occupy
 *          in a sphinx of order n.
 * https://oeis.org/A318778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 36*n^2 - 80*n + 44 for n>1; a(1)=1
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    seq_t nn = (seq_t)n;
    /* Compute 36*nn*nn - 80*nn + 44 */
    seq_t term = 36 * nn * nn;
    term -= 80 * nn;
    term += 44;
    return term;
}
