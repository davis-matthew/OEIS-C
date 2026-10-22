/*
 * A323209  a(n) = hypergeometric([-n, n + 1], [-n - 1], n).
 * https://oeis.org/A323209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} binomial(n+j, n)*(1 - j/(n+1))*n^j
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t binom = 1;          /* binomial(n + j, n) */
    seq_t n_pow = 1;          /* n^j            */
    seq_t denom = (seq_t)(n + 1);  /* n + 1          */

    for (idx_t j = 0; j <= n; ++j) {
        seq_t t = binom * ((seq_t)(denom - j));     /* binom * (n+1-j) */
        seq_t term_scaled = t / denom;              /* / (n+1)        */
        sum += term_scaled * n_pow;                 /* term * n^j     */
        if (j == n) break;                          /* final term done */

        /* update binomial for next j: C(n + j + 1, n) = C(n + j, n) * (n + j + 1) / (j + 1) */
        binom = binom * ((seq_t)(n + j + 1)) / ((seq_t)(j + 1));
        n_pow *= (seq_t)n;                          /* next power of n */
    }
    return sum;
}
