/*
 * A372382  Coefficient of x^n in the expansion of ( (1+x+x^2)^4 / (1+x)^3
 *          )^n.
 * https://oeis.org/A372382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(4*n,k) * binomial(n-k,k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for idx_t and seq_t types provided by the build */

seq_t a(idx_t n)
{
    idx_t kmax = n / 2;
    seq_t c1    = 1;   /* binomial(4*n,0)       */
    seq_t B     = 1;   /* binomial(n,0) = 1     */
    seq_t sum   = 1;   /* term for k = 0         */

    for (idx_t k = 1; k <= kmax; ++k)
    {
        /* update binomial(4*n, k) using recurrence */
        c1 = c1 * (seq_t)(4 * n - k + 1) / k;

        /* update binomial(n-k, k) (which equals binomial(n-k, n-2*k))
           via recurrence: B_k = B_{k-1} * ( (n-2t)*(n-2t-1) ) / ( (n-t)*(k) ),
           where t = k-1 */
        idx_t t   = k - 1;
        seq_t num = (seq_t)(n - 2 * t) * (seq_t)(n - 2 * t - 1);
        seq_t den = (seq_t)(n - t) * (seq_t)k;
        B = B * num / den;

        /* accumulate term */
        sum += c1 * B;
    }

    return sum;
}
