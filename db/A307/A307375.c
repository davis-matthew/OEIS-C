/*
 * A307375  Expansion of Sum_{j>=0} j!*x^j / Product_{k=1..j} (1 - k^2*x).
 * https://oeis.org/A307375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A307375
    a(n) = coefficient of x^n in  Σ_{j≥0} j! x^j / ∏_{k=1..j} (1 – k²x)
    = Σ_{j=0..n} j! * w_j(n–j)
    where w_j(m) satisfies
        w_0(0)=1, w_0(m>0)=0;
        w_j(m) = w_{j-1}(m) + j²·w_j(m–1)   (m≥1)
 */
 
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
 
idx_t _floor_isqrt_u64(idx_t);  /* provided by the build system */
 
/* compute A307375 */
seq_t a(idx_t n)
{
    /* factorials */
    seq_t *fact = (seq_t *)calloc(n + 1, sizeof(seq_t));
    if (!fact) return 0;
    fact[0] = 1;
    for (idx_t i = 1; i <= n; ++i) fact[i] = fact[i - 1] * i;
 
    /* dp array for w_j(m) */
    seq_t *dp   = (seq_t *)calloc(n + 1, sizeof(seq_t));
    if (!dp) { free(fact); return 0; }
    dp[0] = 1;   /* w_0(0)=1 */
 
    seq_t sum = 0;
    for (idx_t j = 0; j <= n; ++j)
    {
        /* add contribution of current j */
        sum += fact[j] * dp[n - j];
        if (j == n) break;
 
        /* update dp for next j+1 */
        seq_t w = (seq_t)(j + 1)*(j + 1);   /* weight j+1 squared */
        for (idx_t m = 1; m <= n; ++m)
        {
            dp[m] = dp[m] + w * dp[m - 1];
        }
    }
 
    free(dp);
    free(fact);
    return sum;
}
