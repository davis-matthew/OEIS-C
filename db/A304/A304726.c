/*
 * A304726  a(n) = n^4 + 4*n^2 + 3.
 * https://oeis.org/A304726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 + 4*n^2 + 3
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn  = (seq_t)n;          /* cast index to the value type   */
    seq_t n2  = sn * sn;           /* n^2                            */
    seq_t n4  = n2 * n2;           /* n^4                            */
    return n4 + 4 * n2 + (seq_t)3; /* n^4 + 4*n^2 + 3                */
}
