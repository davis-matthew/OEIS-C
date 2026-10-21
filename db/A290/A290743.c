/*
 * A290743  Maximum number of distinct Lyndon factors that can appear in words
 *          of length n over an alphabet of size 2.
 * https://oeis.org/A290743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^2 + 16)/8 if n is even a(n) = (2*n^2 + 14)/8 if n is
 *            odd (OEIS A290743)
 * Range:     u8 n <= 11, u16 n <= 191, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_sq   = (seq_t)n * (seq_t)n;     /* n^2 */
    seq_t two_n_sq = n_sq * 2;              /* 2*n^2 */

    if (n & 1)                               /* n odd */
        return (two_n_sq + 14) / 8;
    else                                     /* n even */
        return (two_n_sq + 16) / 8;
}
