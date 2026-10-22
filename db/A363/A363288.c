/*
 * A363288  a(n) = (2*n^3 - n^2 + 3*n - 2)/2.
 * https://oeis.org/A363288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 - n^2 + 3*n - 2)/2
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 1536, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t val = 2 * n3;
    val -= n2;
    val += 3 * nn;
    val -= 2;
    return val / 2;
}
