/*
 * A357745  Numbers on the 8 main spokes of a square spiral with 1 in the
 *          center.
 * https://oeis.org/A357745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+3)^2 / 16) + (1 if n != 1 mod 8)
 * Range:     u8 n <= 60, u16 n <= 1020, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t  = nn + (seq_t)3;
    seq_t result = (t * t) / (seq_t)16;
    if ((n % 8) != 1)
        result += (seq_t)1;
    return result;
}
