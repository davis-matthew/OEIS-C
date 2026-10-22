/*
 * A390345  Upper (2/3)-midsequence of binomial(n,3) and binomial(n,2); see
 *          Comments.
 * https://oeis.org/A390345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((n^3 - n)/9)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_s = (seq_t)n;
    seq_t x = n_s * n_s;       /* n^2 */
    seq_t y = x * n_s;         /* n^3 */
    seq_t val = y - n_s;       /* n^3 - n */
    return (val + 8) / 9;      /* ceil division by 9 */
}
