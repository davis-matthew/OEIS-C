/*
 * A251658  a(n) = Sum_{k=1..n} C(n,k) * sigma(k)^(n-k) for n>=1.
 * https://oeis.org/A251658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} C(n,k) * sigma(k)^(n-k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset starts at 1, but guard against 0 */

    seq_t sum = 0;
    seq_t C   = 1;           /* C(n,0) */

    for (idx_t k = 1; k <= n; ++k) {
        /* update binomial coefficient to C(n,k) */
        C = (C * (seq_t)(n - k + 1)) / (seq_t)k;

        seq_t sigma = sigma_of(k);                     /* sigma(k) */
        seq_t pw    = ipow(sigma, (seq_t)(n - k)); /* sigma(k)^(n-k) */
        seq_t term  = C * pw;                          /* C(n,k)*sigma(k)^(n-k) */

        sum += term;
    }

    return sum;
}
