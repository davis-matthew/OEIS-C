/*
 * A385468  Expansion of e.g.f. 1/(1 - 2 * arctanh(x))^(1/2).
 * https://oeis.org/A385468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} A001147(k)*A111594(n,k) Computed via the
 *            recurrence derived from the e.g.f. a(0) = 1 c[0] = 1 c[n] =
 *            Σ_{odd i≤n} 2 ⋅ c[n-i] ⋅ fact[n] / fact[n-i] ÷ i a[n] = ( c[n] –
 *            Σ_{k=1}^{n-1} a[k]⋅a[n-k]⋅C(n,k) ) / 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Convert to a local index type that fits in size_t for array sizing. */
    idx_t N = n;

    /* Allocate arrays for factorials, auxiliary sequence c, and result a. */
    seq_t *fact = (seq_t *)malloc((size_t)(N + 1) * sizeof(seq_t));
    seq_t *c    = (seq_t *)malloc((size_t)(N + 1) * sizeof(seq_t));
    seq_t *ares = (seq_t *)malloc((size_t)(N + 1) * sizeof(seq_t));

    if (!fact || !c || !ares) {
        /* In the unlikely event of allocation failure we return 0. */
        free(fact); free(c); free(ares);
        return (seq_t)0;
    }

    /* factorial[0] = 1 */
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= N; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }

    /* c[0] = 1 */
    c[0] = (seq_t)1;
    for (idx_t n_ = 1; n_ <= N; ++n_) {
        seq_t sum = (seq_t)0;
        /* sum over odd i */
        for (idx_t i = 1; i <= n_; i += 2) {
            /* prod = fact[n_] / fact[n_-i]  (exact) */
            seq_t prod = fact[n_] / fact[n_ - i];
            /* term = 2 * c[n_-i] * prod / i */
            seq_t term = c[n_ - i];
            term *= prod;
            term *= (seq_t)2;
            term /= (seq_t)i;          /* i is odd → division exact */
            sum += term;
        }
        c[n_] = sum;
    }

    /* a[0] = 1 */
    ares[0] = (seq_t)1;
    for (idx_t n_ = 1; n_ <= N; ++n_) {
        seq_t sub = (seq_t)0;
        for (idx_t k = 1; k <= n_ - 1; ++k) {
            seq_t bin = binomial(n_, k);
            seq_t term = ares[k] * ares[n_ - k] * bin;
            sub += term;
        }
        /* a[n_] = (c[n_] - sub) / 2 */
        seq_t diff = c[n_] - sub;
        ares[n_] = diff / (seq_t)2;
    }

    seq_t result = ares[N];

    free(fact);
    free(c);
    free(ares);
    return result;
}
