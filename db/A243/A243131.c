/*
 * A243131  a(n) = 16*n^5 - 20*n^3 + 5*n.
 * https://oeis.org/A243131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n^5 - 20*n^3 + 5*n
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 48, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;        /* convert index to seq_t */
    seq_t n2 = sn * sn;         /* n^2 */
    seq_t n3 = n2 * sn;         /* n^3 */
    seq_t n5 = n3 * n2;         /* n^5 = n^3 * n^2 */
    return 16 * n5 - 20 * n3 + 5 * sn;
}
