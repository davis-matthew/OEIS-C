/*
 * A292343  The PI index of the Aztec diamond AZ(n) (see the Imran et al.
 *          reference).
 * https://oeis.org/A292343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4/3)*n*(12*n^3 + 44*n^2 + 36*n - 11)
 * Range:     u8 none, u16 n <= 5, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is assumed >= 1 */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t term = 12 * n3 + 44 * n2 + 36 * nn - 11;
    seq_t num  = 4 * nn * term;  /* 4*n*term */
    return num / 3;
}
