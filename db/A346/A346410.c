/*
 * A346410  a(n) = (n!)^2 * Sum_{k=0..n-1} 1 / ((n-k) * k!)^2.
 * https://oeis.org/A346410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^2 * Sum_{k=0..n-1} 1 / ((n-k)*k!)^2 Equivalent form:
 *            a(n) = Σ_{j=1}^{n} ((j-1)! · C(n,j))^2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset 0 */
    if (n == 0) {
        return 0;
    }

    seq_t sum = 0;
    seq_t fact = 1;          /* (j-1)!   for current j, starts with j=1: 0! = 1   */
    seq_t binom = (seq_t)n;  /* binom(n,1) = n   */

    for (idx_t j = 1; j <= n; ++j) {
        /* term = ((j-1)! * binom(n,j))^2 */
        seq_t prod = fact * binom;
        seq_t term = prod * prod;
        sum += term;

        /* Prepare for next j+1 */
        if (j < n) {
            /* fact update: fact *= j   (now fact holds j! for next iteration) */
            fact *= (seq_t)j;
            /* binom update: binom = binom * (n - j) / (j + 1) */
            binom = binom * (seq_t)(n - j) / (j + 1);
        }
    }

    return sum;
}
