/*
 * A365373  a(n) = n*(3*n^4 + 15*n^3 + 25*n^2 - 15*n - 28)/60.
 * https://oeis.org/A365373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3*n^4 + 15*n^3 + 25*n^2 - 15*n - 28)/60
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the integer type used for sequence values. */
    seq_t nn = (seq_t)n;

    /* Compute powers of n needed for the polynomial. */
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */

    /* Evaluate the polynomial in the numerator. */
    seq_t num = nn * ( (seq_t)3 * n4
                      + (seq_t)15 * n3
                      + (seq_t)25 * n2
                      - (seq_t)15 * nn
                      - (seq_t)28 );

    /* Divide by 60; the result is an integer by construction. */
    return num / 60;
}
