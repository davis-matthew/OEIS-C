/*
 * A376901  a(n) = (n*(n-1)+(-1)^n+5)/2.
 * https://oeis.org/A376901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(n-1)+(-1)^n+5)/2
 * Range:     u8 n <= 16, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the only case where n*(n-1) would underflow for unsigned */
    if (n == 0U) {
        return (seq_t)3;
    }

    seq_t nn  = (seq_t)n;
    seq_t nn1 = nn - (seq_t)1;          /* safe because n > 0 */
    seq_t prod = nn * nn1;

    /* (-1)^n is +1 for even n, -1 for odd n.
       Thus the constant to add is 6 when n even, 4 when n odd. */
    seq_t add = (n % 2U == 0U) ? (seq_t)6 : (seq_t)4;

    return (prod + add) / (seq_t)2;
}
