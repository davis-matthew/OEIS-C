/*
 * A371678  G.f. satisfies A(x) = 1 + x * A(x)^3 * (1 + A(x)^(1/2))^2.
 * https://oeis.org/A371678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Sum_{k=0..n} binomial(n,k) *
 *            binomial(6*n+k+2,n)/(6*n+k+2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is O(n) for each call, with no additional assumptions about caller order. */
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    idx_t n64 = n;                          /* aliases for clarity */

    for (idx_t k = 0; k <= n64; ++k) {
        /* binomial(n,k) */
        seq_t binom_nk = binomial(n64, k);
        /* binomial(6*n+k+2, n) */
        idx_t m = 6 * n64 + k + 2;
        seq_t binom_m = binomial((idx_t)m, n64);
        /* denominator */
        seq_t denom = (seq_t)m;

        /* term = 2 * binom_nk */
        seq_t term = binom_nk * (seq_t)2;

        /* reduce term with denom */
        seq_t g1 = gcd(term, denom);
        term /= g1;
        denom /= g1;

        /* reduce binom_m with remaining denom */
        seq_t g2 = gcd(binom_m, denom);
        binom_m /= g2;
        denom /= g2;      /* should now be 1 */

        /* multiply remaining factors */
        term *= binom_m;   /* exact integer division already performed */

        sum += term;
    }
    return sum;
}
