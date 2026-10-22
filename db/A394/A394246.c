/*
 * A394246  Upper (1/3,2/3) midsequence of (floor[n/2]^2) and
 *          (ceiling[n/2]^2); see Comments.
 * https://oeis.org/A394246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil( floor(n/2)^2/3 + 2*ceil(n/2)^2/3 ) * = (
 *            floor(n/2)^2 + 2*ceil(n/2)^2 + 2 ) / 3
 * Range:     u8 n <= 18, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* floor(n/2) */
    idx_t f = n >> 1;
    /* ceil(n/2) */
    idx_t c = n - f;

    /* Compute squares using seq_t to avoid overflow of idx_t */
    seq_t ff = (seq_t)f * (seq_t)f;
    seq_t cc = (seq_t)c * (seq_t)c;

    /* numerator = f^2 + 2*c^2 */
    seq_t numerator = ff + 2 * cc;

    /* ceil(numerator / 3) = (numerator + 2) / 3 */
    return (numerator + 2) / 3;
}
