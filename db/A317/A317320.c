/*
 * A317320  Multiples of 20 and odd numbers interleaved.
 * https://oeis.org/A317320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n, if n is odd; a(n) = 10*n, if n is even
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1)
        return (seq_t)n;          /* n is odd */
    else
        return (seq_t)n * 10;     /* n is even */
}
