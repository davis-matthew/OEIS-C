/*
 * A365726  G.f. satisfies A(x) = 1 + x^3*A(x)^5*(1 + x*A(x)).
 * https://oeis.org/A365726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} binomial(k,n−3k) * binomial(n+2k+1,k) /
 *            (n+2k+1)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t rem = n - 3 * k;
        if (rem > k) continue;                   /* binomial(k,rem)=0 */
        seq_t c1 = binomial(k, rem);
        seq_t c2 = binomial(n + 2 * k + 1, k);
        seq_t num = c1 * c2;
        seq_t denom = (seq_t)(n + 2 * k + 1);
        seq_t term = num / denom;               /* exact division */
        sum += term;
    }
    return sum;
}
