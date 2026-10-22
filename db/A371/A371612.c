/*
 * A371612  G.f. satisfies A(x) = ( 1 + x / (1 - x*A(x)^2) )^2.
 * https://oeis.org/A371612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(4*(n-k)+2,k) *
 *            binomial(n-1,n-k)/(2*(n-k)+1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    seq_t sum = (seq_t)0;

    /* C = binom(n-1, k-1);   r = k-1 */
    seq_t C = (seq_t)1;          /* for k = 1 -> r = 0 */
    idx_t r = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* B = binom(4*(n-k)+2, k);  use the provided routine */
        seq_t B = binomial(4 * (n - k) + 2, k);

        /* denom = 2*(n-k)+1 */
        idx_t denom = 2 * (n - k) + 1;
        seq_t term = (B * C) / (seq_t)denom;
        sum += term;

        /* update C for next k (if any) */
        if (k < n) {
            /* C = C * (n-1 - r) / (r+1);  r = k-1 */
            C = (C * (seq_t)((n - 1) - r)) / (seq_t)(r + 1);
            ++r;
        }
    }

    return sum;
}
