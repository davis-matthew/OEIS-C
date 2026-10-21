/*
 * A244635  a(n) = 29*n^2.
 * https://oeis.org/A244635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 29*n^2
 * Range:     u8 n <= 2, u16 n <= 47, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Perform the computation in seq_t to avoid overflow of intermediate idx_t */
    seq_t t = (seq_t)n * (seq_t)n;   /* n^2 in seq_t */
    return t * ((seq_t)29);          /* multiply by 29 */
}
