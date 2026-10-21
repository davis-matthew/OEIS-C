/*
 * A242852  a(n) = 128*n^7-192*n^5+80*n^3-8*n.
 * https://oeis.org/A242852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 128*n^7-192*n^5+80*n^3-8*n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 11, u64 n <= 282, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = n;            /* cast index to seq_t */
    seq_t tn2 = tn * tn;     /* n^2 */
    seq_t tn4 = tn2 * tn2;   /* n^4 */
    seq_t tn6 = tn4 * tn2;   /* n^6 */

    /* Compute the polynomial inside the parentheses */
    seq_t poly = (128 * tn6) - (192 * tn4) + (80 * tn2) - 8;

    /* Multiply by n to finish: a = n * poly */
    return tn * poly;
}
