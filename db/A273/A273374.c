/*
 * A273374  Squares ending in digit 9.
 * https://oeis.org/A273374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (5*n - (n&1?2:3))^2 for n >= 1
 * Range:     u8 n <= 3, u16 n <= 51, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t t = (seq_t)5 * n;
    t -= (n & 1) ? (seq_t)2 : (seq_t)3;
    return t * t;
}
