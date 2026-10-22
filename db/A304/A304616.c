/*
 * A304616  a(n) = 81*n^2 - 69*n + 24.
 * https://oeis.org/A304616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 81*n^2 - 69*n + 24
 * Range:     u8 n <= 2, u16 n <= 28, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term1 = (seq_t)81 * nn * nn; /* 81 * n^2 */
    seq_t term2 = (seq_t)69 * nn;      /* 69 * n */
    return term1 - term2 + 24;         /* 81*n^2 - 69*n + 24 */
}
