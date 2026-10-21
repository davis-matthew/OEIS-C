/*
 * A258892  Number of (n+2) X (6+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum less than the antidiagonal sum or central row sum
 *          less than the central column sum.
 * https://oeis.org/A258892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = n^4 + 10*n^3 + 182*n^2 + 802*n + 6205 for even n > 3 *
 *            a(n) = n^4 + 10*n^3 + 181*n^2 + 780*n + 6084 for odd n > 3 *
 *            Initial values: * a(1) = 94249, a(2) = 51188, a(3) = 20164
 * Range:     u8 none, u16 none, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly. */
    if (n == 1) return 94249;
    if (n == 2) return 51188;
    if (n == 3) return 20164;

    /* Compute the polynomial for n > 3. */
    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;          /* n^2 */
    seq_t n3  = n2 * nn;          /* n^3 */
    seq_t n4  = n3 * nn;          /* n^4 */

    if ((n & 1) == 0) /* even n */
        return n4 + 10*n3 + 182*n2 + 802*nn + (seq_t)6205;
    else             /* odd n */
        return n4 + 10*n3 + 181*n2 + 780*nn + (seq_t)6084;
}
