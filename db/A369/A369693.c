/*
 * A369693  G.f. satisfies A(x) = 1/(1-x)^4 + x^4*A(x)^4.
 * https://oeis.org/A369693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} binomial(n+8k+3, n-4k) * binomial(4k, k)
 *            / (3k+1) Offset: 0 (so a(0) = 1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 1588 (2026-11-01)
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* Upper bound of k */
    idx_t kmax = n / 4;

    for (idx_t k = 0; k <= kmax; ++k) {
        /* Compute the numerator parts */
        seq_t c1 = binomial(n + 8 * k + 3, n - 4 * k);
        seq_t c2 = binomial(4 * k, k);

        /* Denominator (3*k+1) fits in idx_t */
        seq_t div = (seq_t)(3 * k + 1);

        /* Reduce to keep intermediate values small */
        seq_t g1 = gcd(c1, div);
        c1 /= g1;
        div /= g1;

        seq_t g2 = gcd(c2, div);
        c2 /= g2;
        div /= g2;

        /* At this point div should be 1; division is exact */
        seq_t term = (c1 * c2) / div;
        sum += term;
    }

    return sum;
}
