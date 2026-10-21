/*
 * A259056  a(n) gives the determinant of a bisymmetric n X n matrix involving
 *          the entries 1, 2, ..., A002620(n+1).
 * https://oeis.org/A259056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(1) = 1, a(2n) = (-1)^n * 3 * ∏_{k=2}^{n} (8k + 4) (n ≥ 1)
 *            a(2n-1) = (-1)^{n-1} * ∏_{k=1}^{n-1} (8k + 8) (n ≥ 1)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 13, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base case n = 1 */
    if (n == 1) return (seq_t)1;

    /* Even n: a(2m) with m = n/2  */
    if ((n & 1) == 0) {
        idx_t m = n / 2;
        seq_t prod = (seq_t)3;
        for (idx_t k = 2; k <= m; ++k) {
            prod *= (seq_t)(8 * k + 4);
        }
        if (m & 1) prod = -prod;   /* (-1)^m */
        return prod;
    }

    /* Odd n: a(2m-1) with m = (n+1)/2  */
    idx_t m = (n + 1) / 2;
    seq_t prod = (seq_t)1;
    for (idx_t k = 1; k < m; ++k) {
        prod *= (seq_t)(8 * k + 8);
    }
    if ((m - 1) & 1) prod = -prod;   /* (-1)^{m-1} */
    return prod;
}
