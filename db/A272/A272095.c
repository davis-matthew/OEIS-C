/*
 * A272095  a(n) = Product_{k=0..n} binomial(n^2,k).
 * https://oeis.org/A272095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product_{k=0..n} binomial(n^2, k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n^2 as seq_t */
    seq_t nsq = (seq_t)n * (seq_t)n;

    seq_t prod  = 1;   /* running product a(n) */
    seq_t binom = 1;   /* current binomial C(n^2, k) */

    /* k = 0 term is already binom = 1, prod = 1 */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* binom(k) = binom(k-1) * (n^2 - (k-1)) / k       */
        seq_t numerator   = nsq - (seq_t)(k - 1);
        seq_t denominator = (seq_t)k;
        binom = binom * numerator / denominator;
        prod *= binom;
    }

    return prod;
}
