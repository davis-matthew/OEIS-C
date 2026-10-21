/*
 * A240930  a(n) = n^7 - n^6.
 * https://oeis.org/A240930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 - n^6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 23, u64 n <= 565, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to seq_t */
    seq_t n6 = 1;                 /* will hold n^6 */
    for (int i = 0; i < 6; ++i)   /* compute n^6 */
        n6 *= nn;
    return n6 * nn - n6;          /* n^7 - n^6 */
}
