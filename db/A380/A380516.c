/*
 * A380516  Expansion of e.g.f. exp(x*G(x)^4) where G(x) = 1 + x*G(x)^4 is the
 *          g.f. of A002293.
 * https://oeis.org/A380516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * Sum_{k=0..n-1} binomial(4*n,k)/(n-k-1)! for n >
 *            0 a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t sum = 1;          /* term for k = 0 */
    seq_t binom = 1;        /* binomial(4*n, 0)   */
    seq_t falling = 1;      /* (n-1)_(0) = 1      */

    for (idx_t k = 0; k < n - 1; ++k) {
        /* compute binom(4*n, k+1) from binom(4*n, k) */
        seq_t num = binom * ((seq_t)(4 * n - k));
        binom = num / ((seq_t)(k + 1));

        /* compute falling(n-1, k+1) from falling(n-1, k) */
        falling = falling * ((seq_t)(n - k - 1));

        /* add the term to the sum */
        sum += binom * falling;
    }

    return sum;
}
