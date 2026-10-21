/*
 * A276390  Bisection of A115716.
 * https://oeis.org/A276390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 3 if n is even and n > 0; a(n) = 4 * a(n >> 1)
 *            - 1 if n is odd
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 9335, u64 n <= 9335,
 *            u128 n <= 9335, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;
    if ((n & (idx_t)1) == 0) /* n even */
        return (seq_t)3;
    return (seq_t)4 * a(n >> 1) - (seq_t)1;
}
