/*
 * A393192  a(n) = 32*n^3 - 72*n^2 + 60*n - 19.
 * https://oeis.org/A393192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*n^3 - 72*n^2 + 60*n - 19 (offset 1)
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 512, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = n;          /* cast index to seq_t */
    seq_t n2   = nn * nn;    /* n^2   */
    seq_t n3   = n2 * nn;    /* n^3   */

    return (seq_t)32 * n3 - (seq_t)72 * n2 + (seq_t)60 * nn - (seq_t)19;
}
