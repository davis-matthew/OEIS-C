/*
 * A391678  a(n) = Sum_{k=0..n} 2^k * 3^(n-k) * binomial(k+2,2) *
 *          binomial(k,n-k)^2.
 * https://oeis.org/A391678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} 2^k * 3^(n-k) * binomial(k+2,2) *
 *            binomial(k,n-k)^2 (OEIS A391678) Implementation uses the
 *            recurrence: g(0)=1, for m>=1: g(m) = g(m-1) *
 *            ((n-2m+2)*(n-2m+1)) / (m*(n-m+1)), where g(m) = binomial(n-m,
 *            m). The sum is over m=0..⌊n/2⌋. Powers of 2 and 3 are obtained
 *            from the helper f...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;          /* result accumulator */
    seq_t g   = 1;          /* g(m) = binomial(n-m, m) for current m */
    idx_t half = n / 2;     /* loop limit: m in [0, floor(n/2)] */

    for (idx_t m = 0; m <= half; ++m)
    {
        idx_t k = n - m;                            /* k = n-m */

        /* binomial(k+2, 2) = (k+2)(k+1)/2 */
        seq_t bin2 = ((seq_t)(k + 2) * (seq_t)(k + 1)) / 2;

        seq_t pow2k = pow2(k);                      /* 2^k */
        seq_t pow3m = pow3(m);                      /* 3^m */

        /* term = g^2 * bin2 * 2^k * 3^m */
        seq_t term = g * g;
        term *= bin2;
        term *= pow2k;
        term *= pow3m;

        sum += term;

        /* update g for next m (if any) */
        if (m < half)
        {
            idx_t next = m + 1;
            seq_t num1 = (seq_t)(n - 2 * next + 2);
            seq_t num2 = (seq_t)(n - 2 * next + 1);
            seq_t denom = (seq_t)next * (seq_t)(n - next + 1);
            g = g * num1 * num2 / denom;
        }
    }

    return sum;
}
