/*
 * A354594  a(n) = n^2 + 2*floor(n/2)^2.
 * https://oeis.org/A354594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + 2*floor(n/2)^2
 * Range:     u8 n <= 13, u16 n <= 209, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;          /* cast index to sequence type */
    seq_t lh = tn / 2;            /* floor(n/2) */
    return tn * tn + 2 * lh * lh;
}
