/*
 * A306789  a(n) = Product_{k=0..n} binomial(n + k, n).
 * https://oeis.org/A306789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} binomial(n + k, n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;  /* binomial(0,0) = 1 */

    seq_t result = (seq_t)1;      /* accumulates the product */
    seq_t binom  = (seq_t)1;      /* current binomial C(n + k, n)   */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update binomial coefficient:
           C(n + k, n) = C(n + k - 1, n) * (n + k) / k    */
        binom = binom * ((seq_t)n + (seq_t)k) / (seq_t)k;
        /* Multiply into the overall product */
        result *= binom;
    }

    /* For k = 0 we have C(n, n) = 1, already accounted for by the
       initial value of result. */
    return result;
}
