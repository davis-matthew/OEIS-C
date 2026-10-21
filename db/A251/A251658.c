/*
 * A251658  a(n) = Sum_{k=1..n} C(n,k) * sigma(k)^(n-k) for n>=1.
 * https://oeis.org/A251658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} C(n,k) * sigma(k)^(n-k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow, binomial, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset starts at 1, but guard against 0 */

    seq_t sum = 0;
    seq_t C   = 1;           /* C(n,0) */

    for (idx_t k = 1; k <= n; ++k) {
        /* update binomial coefficient to C(n,k) */
        C = (C * (seq_t)(n - k + 1)) / (seq_t)k;

        seq_t sigma = sigma_of(k);                     /* sigma(k) */
        seq_t pw    = big_pow(sigma, (seq_t)(n - k)); /* sigma(k)^(n-k) */
        seq_t term  = C * pw;                          /* C(n,k)*sigma(k)^(n-k) */

        sum += term;
    }

    return sum;
}
