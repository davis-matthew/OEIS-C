/*
 * A290313  Fourth diagonal sequence of the Sheffer triangle A094816 (special
 *          Charlier).
 * https://oeis.org/A290313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (48 + 256*n + 422*n^2 + 303*n^3 + 105*n^4 + 17*n^5 + n^6)
 *            / 48
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 37, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n0 = (seq_t)n;        /* n^1 */
    seq_t n1 = n0;
    seq_t n2 = n1 * n1;         /* n^2 */
    seq_t n3 = n2 * n1;         /* n^3 */
    seq_t n4 = n3 * n1;         /* n^4 */
    seq_t n5 = n4 * n1;         /* n^5 */
    seq_t n6 = n5 * n1;         /* n^6 */

    seq_t num = ((seq_t)48)
              + ((seq_t)256 * n1)
              + ((seq_t)422 * n2)
              + ((seq_t)303 * n3)
              + ((seq_t)105 * n4)
              + ((seq_t)17  * n5)
              + n6;
    return num / ((seq_t)48);
}
