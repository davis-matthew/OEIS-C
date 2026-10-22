/*
 * A360474  E.g.f. satisfies A(x) = exp( x * A(x)^2 * exp(x * A(x)^2) ).
 * https://oeis.org/A360474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k^(n-k) * (2*n+1)^(k-1) * binomial(n,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Handle n = 0 separately: the formula gives 1. */
    if (n == 0) return (seq_t)1;

    seq_t result = 0;
    seq_t two_n_plus_one = (seq_t)2 * (seq_t)n + 1;
    seq_t pow2 = 1; /* (2*n+1)^(k-1) for current k; starts with k=1 */

    for (idx_t k = 1; k <= n; ++k) {
        if (k > 1) {
            /* Increase exponent by 1 */
            pow2 *= two_n_plus_one;
        }

        seq_t bin = binomial(n, k);                           /* C(n,k) */
        seq_t powk = big_pow((seq_t)k, (seq_t)(n - k));       /* k^(n-k) */
        seq_t term = bin * powk;                              /* combine bin and power */
        term *= pow2;                                         /* multiply by (2n+1)^(k-1) */
        result += term;                                       /* accumulate */
    }

    return result;
}
