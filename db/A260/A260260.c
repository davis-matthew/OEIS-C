/*
 * A260260  a(n) = n*(16*n^2 - 21*n + 7)/2.
 * https://oeis.org/A260260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(16*n^2 - 21*n + 7)/2
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 645, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t temp = (seq_t)16 * nn * nn          /* 16*n^2   */
               - (seq_t)21 * nn              /*  -21*n   */
               + (seq_t)7;                   /*   + 7    */
    return nn * temp / (seq_t)2;
}
