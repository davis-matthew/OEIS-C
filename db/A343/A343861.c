/*
 * A343861  Coefficient triangle of generalized Laguerre polynomials
 *          n!*L(n,n,x) (rising powers of x).
 * https://oeis.org/A343861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = (-1)^k * n! * binomial(2*n, n-k) / k! where n is the row
 *            index, and k = n - m*(m+1)/2 with m = floor((sqrt(8*n+1)-1)/2).
 *            This produces the sequence A343861
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 44, i64 n <= 119, i128 n <= 324,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find row m and column k from linear index n.  Row m has m+1 entries
       and the total entries up to row m-1 is T_m = m*(m+1)/2.  */
    idx_t m = 0;
    idx_t start = 0;        /* cumulative entries before the current row    */
    while (start + (m + 1) <= n) {
        start += m + 1;
        ++m;
    }
    idx_t k = n - start;

    /* Compute n! / k! = product_{i=k+1}^{m} i  (empty product = 1).   */
    seq_t fact_part = 1;
    for (idx_t i = k + 1; i <= m; ++i) {
        fact_part = fact_part * (seq_t)i;
    }

    /* Compute binomial(2*m, m-k)
       = product_{i=1}^{m-k} (m+k+i) / i
       The division is exact at every step.                  */
    seq_t binom_part = 1;
    for (idx_t i = 1; i <= m - k; ++i) {
        binom_part = binom_part * (seq_t)(m + k + i);
        binom_part = binom_part / (seq_t)i;
    }

    seq_t res = fact_part * binom_part;
    if (k & 1) {          /* apply sign (-1)^k */
        res = -res;
    }
    return res;
}
