/*
 * A375226  Expansion of e.g.f. exp( (x / (1-x))^3 ) / (1-x)^2.
 * https://oeis.org/A375226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/3⌋} binomial(n+1, n-3*k)/k!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* factorial of n */
    seq_t fact_n = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact_n *= (seq_t)i;

    seq_t sum   = 0;
    seq_t binom = (seq_t)(n + 1);   /* binomial(n+1,3*0+1) = n+1 */
    seq_t kfact = 1;                /* 0! = 1 */
    idx_t maxk  = n / 3;

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term = binom * fact_n;
        term /= kfact;
        sum += term;

        if (k == maxk)
            break;

        /* increase k! */
        kfact *= (seq_t)(k + 1);

        /* move from r=3*k+1 to r=3*(k+1)+1 = r+3 */
        idx_t r = 3 * k + 1;
        /* C(n+1, r+3) = C(n+1, r) *
                           (n+1-r)/(r+1) *
                           (n-r)/(r+2) *
                           (n-r-1)/(r+3) */
        seq_t temp = binom;
        temp *= (seq_t)(n + 1 - r);
        temp /= (seq_t)(r + 1);
        temp *= (seq_t)(n - r);
        temp /= (seq_t)(r + 2);
        temp *= (seq_t)(n - r - 1);
        temp /= (seq_t)(r + 3);
        binom = temp;
    }

    return sum;
}
