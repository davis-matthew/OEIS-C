/*
 * A245374  G.f.: Sum_{n>=0} x^n / ( (1+x)^(n+1) * (1 - 3*(n+1)*x) ).
 * https://oeis.org/A245374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = Σ_{m=0..⌊n/2⌋} 3^{n−m} * m! * S(n−m+1 , m+1) * where
 *            S(p,q) are Stirling numbers of the second kind. * The
 *            implementation computes Stirling numbers row‑by‑row and *
 *            accumulates the sum, thus running in O(n²) time and O(n) memory
 *            * (well under the limits for n ≤ 10 000)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build. */

seq_t a(idx_t n)
{
    /* floor(n/2) */
    idx_t mmax = n / 2;

    /* factorials 0! … mmax!  */
    seq_t *fact = (seq_t *)calloc(mmax + 1, sizeof(seq_t));
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= mmax; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }

    /* powers of 3: 3^e for e = 0 … n  */
    seq_t *pow3 = (seq_t *)calloc(n + 1, sizeof(seq_t));
    pow3[0] = (seq_t)1;
    for (idx_t e = 1; e <= n; ++e) {
        pow3[e] = pow3[e - 1] * (seq_t)3;
    }

    /* two rows of Stirling numbers of the second kind */
    seq_t *prev = (seq_t *)calloc(mmax + 2, sizeof(seq_t));
    seq_t *cur  = (seq_t *)calloc(mmax + 2, sizeof(seq_t));
    prev[0] = (seq_t)1;           /* S(0,0)=1 */

    seq_t total = (seq_t)0;

    for (idx_t p = 1; p <= n + 1; ++p) {
        idx_t maxq = (p <= (idx_t)mmax + 1) ? p : (idx_t)mmax + 1;
        cur[0] = (seq_t)0;
        for (idx_t q = 1; q <= maxq; ++q) {
            /* cur[q] = q * prev[q] + prev[q-1] */
            seq_t term1 = prev[q] * (seq_t)q;
            cur[q] = term1 + prev[q - 1];
        }

        /* contribution for this diagonal element if within range */
        idx_t m = n - p + 1;
        if (m <= (idx_t)mmax && m <= n) {
            seq_t stir = cur[(idx_t)m + 1];          /* S(n-m+1, m+1) */
            seq_t pow   = pow3[(idx_t)n - m];
            seq_t factm = fact[m];
            seq_t term  = factm * pow;
            term = term * stir;
            total = total + term;
        }

        /* prepare for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    free(fact);
    free(pow3);
    free(prev);
    free(cur);

    return total;
}
