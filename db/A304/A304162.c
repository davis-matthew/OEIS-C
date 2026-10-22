/*
 * A304162  a(n) = n^4 - 3*n^3 + 9*n^2 - 7*n + 5 (n>=1).
 * https://oeis.org/A304162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 - 3*n^3 + 9*n^2 - 7*n + 5
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n1 = (seq_t)n;            /* n */
    seq_t n2 = n1 * n1;             /* n^2 */
    seq_t n3 = n2 * n1;             /* n^3 */
    seq_t n4 = n2 * n2;             /* n^4 */

    /* Compute: n^4 + 9*n^2 + 5 - 7*n - 3*n^3   */
    seq_t result = n4 + (seq_t)9 * n2 + (seq_t)5;
    result -= (seq_t)7 * n1;
    result -= (seq_t)3 * n3;
    return result;
}
