/*
 * A389090  a(n) = n^4 - 4*(n^3-n)/3.
 * https://oeis.org/A389090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 - 4*(n^3 - n)/3
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;
    seq_t n3  = n2 * nn;
    seq_t n4  = n3 * nn;
    seq_t sub = ((n3 - nn) * (seq_t)4) / (seq_t)3;
    return n4 - sub;
}
