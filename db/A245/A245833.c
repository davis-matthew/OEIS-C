/*
 * A245833  The Szeged index of the triangle-shaped benzenoid T_n (see Fig.
 *          5.7 of the Diudea et al. reference).
 * https://oeis.org/A245833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6+12*n^5+49*n^4+84*n^3+58*n^2+12*n)/4
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;
    seq_t n2 = nn * nn;              /* n^2 */
    seq_t n3 = n2 * nn;              /* n^3 */
    seq_t n4 = n3 * nn;              /* n^4 */
    seq_t n5 = n4 * nn;              /* n^5 */
    seq_t n6 = n5 * nn;              /* n^6 */
    seq_t t  = n6 + 12 * n5 + 49 * n4 + 84 * n3 + 58 * n2 + 12 * nn;
    return t / 4;
}
