/*
 * A396592  Expansion of e.g.f. F_2(x)/x and F_k(x) is the k-th iteration of
 *          x*G(x) with G(x) = exp(x*G(x)^4).
 * https://oeis.org/A396592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n}
 *            (k+1)*(4*k+1)^(k-1)*(4*n-3*k+1)^(n-k-1)*binomial(n,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    seq_t sum   = 0;
    seq_t binom = 1;                     /* C(n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        if (k == n) {
            /* For k = n the term simplifies to (4*n+1)^(n-1) */
            sum += seq_pow((seq_t)(4 * n + 1), n - 1);
            break;
        }

        /* (k+1) * binomial(n,k) */
        seq_t term = binom;
        term *= (seq_t)(k + 1);

        /* (4*k+1)^(k-1)  with 0^(-1) treated as 1 (k==0) */
        seq_t pow1 = 1;
        if (k > 0)
            pow1 = seq_pow((seq_t)(4 * k + 1), k - 1);

        /* (4*n-3*k+1)^(n-k-1)   (n-k-1 >= 0 for k < n) */
        seq_t pow2 = seq_pow((seq_t)(4 * n - 3 * k + 1), n - k - 1);

        term *= pow1;
        term *= pow2;

        sum += term;

        /* Update binomial coefficient for the next k */
        binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
    }

    return sum;
}
