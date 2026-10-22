/*
 * A367733  Numbers k such that the sum of digits of k is equal to the sum of
 *          digits of k+9.
 * https://oeis.org/A367733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = n + floor((n-1)/9) + floor((n-1)/81)*10
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t m  = (seq_t)(n - 1);
    seq_t t9  = m / 9;
    seq_t t81 = m / 81;
    return nn + t9 + t81 * 10;
}
