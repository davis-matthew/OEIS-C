/*
 * A245347  Sum of digits of n written in fractional base 8/3.
 * https://oeis.org/A245347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A007953(A024645(n)) // sum of the digits of the
 *            representation of n in base 8/3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    while (n)
    {
        sum += (seq_t)(n & 7u);   /* n mod 8  (least significant digit) */
        n   = (n >> 3) * 3;       /* n = (n / 8) * 3  (next value)    */
    }
    return sum;
}
