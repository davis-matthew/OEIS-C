/*
 * A365268  G.f. satisfies A(x) = 1 + x*A(x)^2*(1 + x^3*A(x)^2).
 * https://oeis.org/A365268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} binomial(n-3*k, k) * binomial(2*n-4*k+1,
 *            n-3*k)/(2*n-4*k+1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t maxk = n / 4;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t n1 = n - 3 * k;
        idx_t n2 = 2 * n - 4 * k + 1;

        seq_t t1 = binomial(n1, k);
        seq_t t2 = binomial(n2, n1);

        seq_t prod = t1 * t2;
        seq_t denom = (seq_t)(n2);  /* n2 = 2*n-4*k+1 */
        prod = prod / denom;

        result += prod;
    }
    return result;
}
