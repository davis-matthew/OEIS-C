/*
 * A330260  a(n) = n! * Sum_{k=0..n} binomial(n,k) * n^(n - k) / k!.
 * https://oeis.org/A330260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^2 * n^k * k!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t binom_sq = 1;   /* C(n,0)^2 */
    seq_t pow_n = 1;      /* n^0 */
    seq_t fact_k = 1;     /* 0! */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term = binom_sq * pow_n;
        term = term * fact_k;
        sum += term;

        if (k == n) break;   /* last term processed */

        /* Update binomial square for k+1:
           binom_{k+1}^2 = binom_k^2 * (n-k)^2 / (k+1)^2  */
        seq_t num = (seq_t)(n - k);
        num = num * num;          /* (n-k)^2 */
        seq_t den = (seq_t)(k + 1);
        den = den * den;          /* (k+1)^2 */
        binom_sq = binom_sq * num;
        binom_sq = binom_sq / den; /* exact division */

        pow_n  = pow_n * (seq_t)n;             /* n^{k+1} */
        fact_k = fact_k * (seq_t)(k + 1);      /* (k+1)! */
    }

    return sum;
}
