/*
 * A337745  G.f.: Sum_{n>=0} (1 + x^n*(1+x)^n)^n * x^n.
 * https://oeis.org/A337745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 92, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{L=0..n} Σ_{k=0..⌊n/L⌋‑1} C(L, k) · C(L·k, n – L·(k+1))
 * 
 * This implements the closed‐form derived from the generating function
 * G(x) = Σ_{L≥0} (1 + x^L·(1+x)^L)^L · x^L.
 * C(n, k) is the usual binomial coefficient, zero when k<0 or k>n.
 * For n=0 the value is 1.
 */

#include <stdint.h>

/* helper: binomial coefficient C(m, k) for small k, using only seq_t arithmetic */
static seq_t binom_small(idx_t m, idx_t k)
{
    if (k > m) return 0;
    if (k == 0 || k == m) return 1;
    if (k > m - k) k = m - k;          /* use the smaller side */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(m - k + i);  /* multiply by numerator */
        res = res / (seq_t)i;           /* exact division by denominator */
    }
    return res;
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 1;          /* a(0) = 1 */

    seq_t result = 0;

    for (idx_t L = 1; L <= n; ++L) {
        /* k must satisfy L*(k+1) ≤ n  →  k ≤ n/L – 1 */
        idx_t maxK = n / L;        /* integer division */
        if (maxK == 0) continue;   /* no valid k for this L */

        /* iterate k = 0 .. maxK-1 */
        for (idx_t k = 0; k < maxK; ++k) {
            idx_t m = n - L * (k + 1);          /* remaining exponent */
            if (m > L * k) continue;           /* C(L*k, m) would be 0 */

            seq_t c1 = binom_small(L, k);
            seq_t c2 = binom_small(L * k, m);

            if (c1 == 0 || c2 == 0) continue;

            result += c1 * c2;
        }
    }
    return result;
}
