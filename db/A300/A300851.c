/*
 * A300851  Number of 6-cycles in the n-transposition graph.
 * https://oeis.org/A300851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*binomial(n,3)*(n^3+9*n^2+16*n-152)/4
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 3, binomial(n,3) = 0 -> a(n) = 0 */
    if (n < 3) {
        return 0;
    }

    /* Compute n! */
    seq_t fac = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fac *= (seq_t)i;
    }

    /* Compute binomial(n,3) = n*(n-1)*(n-2)/6 */
    seq_t c3 = ((seq_t)n * (seq_t)(n - 1) * (seq_t)(n - 2)) / 6;

    /* Compute polynomial n^3 + 9*n^2 + 16*n - 152 */
    seq_t p = (seq_t)n;
    seq_t poly = p * p * p + (seq_t)9 * p * p + (seq_t)16 * p - (seq_t)152;

    /* a(n) = fac * c3 * poly / 4 */
    seq_t res = fac * c3;
    res *= poly;
    res /= 4;

    return res;
}
