/*
 * A325998  G.f.: Sum_{n>=0} (n+1)*(n+2)/2 * (x + x^n)^n.
 * https://oeis.org/A325998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = Sum_{m>=1} binom(m+2,2) * binom(m, (m^2-n)/(m-1)) 
   when (m^2-n) is divisible by (m-1) and 0 <= (m^2-n)/(m-1) <= m;
   a(0)=1. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* small binomial coefficient: binom(n,k) */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;            /* a(0)=1 */

    seq_t ans = 0;

    /* start m at ceil(sqrt(n)) to ensure m^2 >= n */
    seq_t sq = floor_sqrt((seq_t)n);
    idx_t m_start = (idx_t)sq;
    if ((seq_t)m_start * (seq_t)m_start < (seq_t)n) ++m_start;

    for (idx_t m = m_start; m <= n; ++m) {
        /* binom(m+2,2) = (m+2)(m+1)/2 */
        seq_t coeff = ((seq_t)(m + 2) * (seq_t)(m + 1)) / 2;

        if (m == 1) {                      /* m=1 gives two equal terms */
            if (n == 1) ans += coeff * 2;  /* binom(1,0)+binom(1,1)=2 */
            continue;
        }

        idx_t delta = m * m - n;
        if (delta % (m - 1) != 0) continue;
        idx_t k = delta / (m - 1);
        if (k > m) continue;

        seq_t bm = binom_small(m, k);
        ans += coeff * bm;
    }

    return ans;
}
