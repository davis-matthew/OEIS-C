/*
 * A363478  E.g.f. satisfies A(x) = exp(x * (1 + x) * A(x)^3).
 * https://oeis.org/A363478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=ceil(n/2)}^n (3*k+1)^(k-1) * * binomial(k,
 *            n-k) / k! (OEIS A363478)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the trivial first terms */
    if (n <= 1)
        return (seq_t)1;

    /* 1. Compute factorial(n) */
    seq_t fac_n = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        fac_n *= (seq_t)i;

    /* 2. Initialize variables for the summation */
    seq_t sum = (seq_t)0;
    idx_t k_start = (n + 1) / 2;          /* ceil(n/2) */
    idx_t m = 2 * k_start - n;            /* 0 or 1 for the first term */

    /* denominator part: (n-k)! for k = k_start  */
    seq_t denom1 = (seq_t)1;
    idx_t nk = n - k_start;
    for (idx_t i = 2; i <= nk; ++i)
        denom1 *= (seq_t)i;

    /* (2k-n)! part, starting with m! */
    seq_t factorial_m = (seq_t)1;
    for (idx_t i = 2; i <= m; ++i)
        factorial_m *= (seq_t)i;

    /* 3. Main loop over k */
    for (idx_t k = k_start; k <= n; ++k) {
        seq_t pow_term = big_pow((seq_t)(3 * k + 1), (seq_t)(k - 1));

        seq_t tmp = fac_n;
        tmp /= denom1;            /* fac_n / (n-k)!    */
        tmp /= factorial_m;       /* ... / (2k-n)!     */

        seq_t term = pow_term * tmp;
        sum += term;

        if (k == n)               /* last iteration: skip updates */
            break;

        /* Update denominator for next k */
        denom1 /= (seq_t)(n - k);

        /* Update factorial_m: increase m by 2 and multiply by new factors */
        m += 2;
        factorial_m *= (seq_t)(m - 1);
        factorial_m *= (seq_t)m;
    }

    return sum;
}
