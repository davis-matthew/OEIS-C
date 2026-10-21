/*
 * A243717  Number of inequivalent (mod D_4) ways to place 2 nonattacking
 *          knights on an n X n board.
 * https://oeis.org/A243717
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 - 2*n^2 + 20*n - 16 + IF(MOD(n, 2) = 1)*(2*n^2 - 4*n
 *            - 1))/16
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 353, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 2 the sequence is undefined; return 0 to avoid underflow. */
    if (n < 2) return 0;

    seq_t nn  = (seq_t)n;          /* n */
    seq_t n2  = nn * nn;           /* n^2 */
    seq_t n4  = n2 * n2;           /* n^4 */

    /* Base expression: n^4 - 2*n^2 + 20*n - 16 */
    seq_t term = n4 - 2 * n2 + 20 * nn - 16;

    /* Add the extra term if n is odd */
    if ((n & 1) == 1) {
        term += 2 * n2 - 4 * nn - 1;
    }

    return term / 16;
}
