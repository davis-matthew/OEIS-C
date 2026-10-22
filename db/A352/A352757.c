/*
 * A352757  a(n) = (3*(2*n - 1)^2*((2*n - 1)^2 + 2) + 2*n + 1)/2 for n > 0.
 * https://oeis.org/A352757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*(2*n - 1)^2*((2*n - 1)^2 + 2) + 2*n + 1)/2
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 115, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;                       /* convert idx_t to seq_t          */
    seq_t two_n_minus1 = nn * ((seq_t)2) - 1;          /* 2*n - 1                     */
    seq_t m2 = two_n_minus1 * two_n_minus1;           /* (2*n - 1)^2                 */
    seq_t expr = (seq_t)3 * m2 * (m2 + ((seq_t)2)) + nn * ((seq_t)2) + 1;
    return expr / ((seq_t)2);
}
