/*
 * A269680  Number of length-5 0..n arrays with no repeated value differing
 *          from the previous repeated value by other than plus or minus one
 *          modulo n+1.
 * https://oeis.org/A269680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 83, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A269680
 * Formula: a(1) = 14,
 *          a(n) = n^5 + 5*n^4 + 4*n^3 + 6*n^2 + 4*n - 2   for n > 1
 * (Offset 1)
 */

seq_t a(idx_t n)
{
    /* Ensure we only handle n >= 1; behaviour for n < 1 is undefined. */
    if (n == 1) {
        return 14;
    }

    /* Cast the index once to avoid repeated conversions. */
    seq_t nn = (seq_t)n;

    /* Compute successive powers. */
    seq_t n2 = nn * nn;        /* n^2   */
    seq_t n3 = n2 * nn;        /* n^3   */
    seq_t n4 = n3 * nn;        /* n^4   */
    seq_t n5 = n4 * nn;        /* n^5   */

    /* Evaluate the polynomial. */
    return n5 + 5 * n4 + 4 * n3 + 6 * n2 + 4 * nn - (seq_t)2;
}
