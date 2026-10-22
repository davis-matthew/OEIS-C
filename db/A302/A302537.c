/*
 * A302537  a(n) = (n^2 + 13*n + 2)/2.
 * https://oeis.org/A302537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 13*n + 2) / 2
 * Range:     u8 n <= 10, u16 n <= 354, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*   (n^2 + 13*n + 2) / 2   =   (n*(n + 13) + 2) / 2   */
    seq_t num = (seq_t)n * ((seq_t)n + 13) + 2;
    return num / 2;
}
