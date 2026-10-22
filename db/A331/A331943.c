/*
 * A331943  a(n) = n^2 + 1 - ceiling((n + 2)/3).
 * https://oeis.org/A331943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 + 1 - ceil((n + 2)/3)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t part = (nn + 4) / 3;   /* ceil((n+2)/3) */
    return nn * nn + 1 - part;
}
