/*
 * A322597  a(n) = (4*n^3 - 6*n^2 + 20*n + 3)/3.
 * https://oeis.org/A322597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 - 6*n^2 + 20*n + 3)/3
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* Promote index to seq_t */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */
    seq_t num = 4 * n3 - 6 * n2 + 20 * nn + 3; /* numerator */
    return num / 3;               /* integer division yields a(n) */
}
