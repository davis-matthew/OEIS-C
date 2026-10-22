/*
 * A392105  Upper (1,1/2) midsequence of (n^2) and (n^3); see Comments.
 * https://oeis.org/A392105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + ceil(n^3/2)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_sq  = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t n_cu  = n_sq * (seq_t)n;              /* n^3 */
    seq_t ceil_half = (n_cu + 1) / 2;           /* ceil(n^3/2) */
    return n_sq + ceil_half;                    /* a(n) */
}
