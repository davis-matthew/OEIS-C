/*
 * A365205  Centered pentachoral numbers.
 * https://oeis.org/A365205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n^4)/2 - 5*n^3 + (15*n^2)/2 - 5*n + 1
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 203, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  An explicit polynomial evaluation.  The numerator
        5*n^4 - 10*n^3 + 15*n^2 - 10*n is always even,
        so dividing by 2 yields an integer.  */
    seq_t s = (seq_t)n;
    seq_t n2 = s * s;          /* n^2   */
    seq_t n3 = n2 * s;         /* n^3   */
    seq_t n4 = n3 * s;         /* n^4   */

    seq_t num = 5 * n4 - 10 * n3 + 15 * n2 - 10 * s;
    return num / 2 + 1;
}
