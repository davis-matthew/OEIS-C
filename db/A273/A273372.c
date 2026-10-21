/*
 * A273372  Squares ending in digit 1.
 * https://oeis.org/A273372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ((10*n - 5 + 3*(-1)^n)/2)^2
 * Range:     u8 n <= 3, u16 n <= 51, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    int sign = (n & 1) ? -1 : 1;          /* (-1)^n */
    seq_t tmp  = (seq_t)n * 10 - 5 + (sign == 1 ? (seq_t)3 : (seq_t)(-3));
    seq_t half = tmp >> 1;                /* divide by 2 */
    return half * half;                   /* square the result */
}
