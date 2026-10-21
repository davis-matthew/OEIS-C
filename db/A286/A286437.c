/*
 * A286437  Number of ways to tile an n X n X n triangular area with two 2 X 2
 *          X 2 triangular tiles and an appropriate number (= n^2-8) of 1 X 1
 *          X 1 tiles.
 * https://oeis.org/A286437
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 - 6*n^3 + 5*n^2 + 30*n - 54)/2 for n >= 3
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 304, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;               /* Undefined for n < 3; return 0 */

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;         /* n^2 */
    seq_t n3 = n2 * nn;         /* n^3 */
    seq_t n4 = n3 * nn;         /* n^4 */

    seq_t num = n4
              - (seq_t)6 * n3
              + (seq_t)5 * n2
              + (seq_t)30 * nn
              - (seq_t)54;

    return num / (seq_t)2;
}
