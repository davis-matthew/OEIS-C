/*
 * A286444  Number of non-equivalent ways to tile an n X n X n triangular area
 *          with two 2 X 2 X 2 triangular tiles and an appropriate number (=
 *          n^2-8) of 1 X 1 X 1 tiles.
 * https://oeis.org/A286444
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 - 6*n^3 + 11*n^2 - 12)/12 + IF(n mod 2 = 1, (-n +
 *            2)/2, 0)
 * Range:     u8 n <= 9, u16 n <= 31, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;  // undefined below offset, but return 0

    seq_t nn = (seq_t)n;

    seq_t term = ( (nn*nn*nn*nn)                /* n^4 */
                  - (6*nn*nn*nn)              /* -6 n^3 */
                  + (11*nn*nn)                /* +11 n^2 */
                  - (12) ) / 12;              /* -12   divided by 12 */

    /* Add the odd–n correction: -(n-2)/2 for odd n */
    if ((n & 1) != 0)
        term -= (nn - 2) / 2;

    return term;
}
