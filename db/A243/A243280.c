/*
 * A243280  Expansion of log'(1/2-sqrt((5*x+2*sqrt(1-4*x)-2)/x)/2).
 * https://oeis.org/A243280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..n} binomial(2*k-1,k) * binomial(2*n,n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle n = 0 separately – the loop below will work but this makes it
       explicit and avoids unnecessary arithmetic. */
    if (n == 0)
        return 1;

    seq_t sum  = 0;
    seq_t curC = 1;                 /* C(2*k-1,k) for current k */
    seq_t bin;                      /* C(2*n,n-k) for current k */

    /* Initial values for k = 0 */
    bin = binomial((idx_t)(2 * n), n);   /* C(2*n,n) */
    sum += bin;                          /* term for k = 0 */

    /* Loop over k = 1 .. n */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update binomial(2*k-1,k) */
        if (k == 1) {
            /* C(1,1) = 1 – already set in curC */
            curC = 1;
        } else {
            /* C(2k-1,k) = C(2(k-1)-1,k-1) * (2k-1)*(2k-2) / (k*(k-1)) */
            seq_t num   = curC * (seq_t)(2 * k - 1);
            num        = num * (seq_t)(2 * k - 2);
            seq_t denom = (seq_t)(k * (k - 1));
            curC = num / denom;
        }

        /* Update binomial(2*n, n-k) using the recurrence
           C(2n, n-k) = C(2n, n-(k-1)) * (n-k+1) / (n+k)  */
        seq_t factor_num = (seq_t)(n - k + 1);
        seq_t factor_den = (seq_t)(n + k);
        bin = bin * factor_num / factor_den;

        /* Accumulate the term */
        sum += curC * bin;
    }

    return sum;
}
