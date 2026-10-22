/*
 * A376709  G.f.: Sum_{k>=0} x^(k^2) * Product_{j=1..k} 1/(1 - x^j)^3.
 * https://oeis.org/A376709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A376709
   a(n) = coefficient of x^n in  Σ_{k≥0} x^{k^2} · Π_{j=1..k} (1 - x^j)^(-3)   */
/* The algorithm computes for each k the polynomial
   f_k(x)=Π_{j=1..k}(1-x^j)^(-3) by convolution with
   (1-x^k)^(-3)=Σ_{t≥0} C(t+2,2) x^{t·k}.   */

#include <stddef.h>
#include <stdlib.h>

static inline idx_t floor_sqrt(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n) ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0)=1 */

    const idx_t N = n;
    const idx_t maxK = floor_sqrt(N);     /* maximum k with k^2 ≤ n */

    /* Allocate three temporary arrays of length N+1 */
    seq_t *dp     = (seq_t *)malloc(sizeof(seq_t) * (N + 1));
    seq_t *dp_new = (seq_t *)malloc(sizeof(seq_t) * (N + 1));
    seq_t *res    = (seq_t *)malloc(sizeof(seq_t) * (N + 1));
    if (!dp || !dp_new || !res) return (seq_t)0;  /* insufficient memory */

    /* initialise */
    for (idx_t i = 0; i <= N; ++i) {
        dp[i]     = (seq_t)0;
        dp_new[i] = (seq_t)0;
        res[i]    = (seq_t)0;
    }
    dp[0] = (seq_t)1;                       /* f_0(x) = 1 */
    res[0] = (seq_t)1;                      /* contribution from k=0 */

    for (idx_t k = 1; k <= maxK; ++k) {
        /* Build dp_new = dp * (1 - x^k)^(-3)        */
        for (idx_t m = 0; m <= N; ++m) {
            seq_t sum = (seq_t)0;
            idx_t maxT = m / k;
            for (idx_t t = 0; t <= maxT; ++t) {
                /* binomial coefficient C(t+2,2) = (t+2)(t+1)/2  */
                unsigned long long c = ((unsigned long long)t + 2) * ((unsigned long long)t + 1) / 2;
                sum += dp[m - t * k] * (seq_t)c;
            }
            dp_new[m] = sum;
        }

        /* copy dp_new back into dp for next iteration */
        for (idx_t m = 0; m <= N; ++m) dp[m] = dp_new[m];

        /* add contribution of f_k(x) shifted by k^2 to the result */
        idx_t shift = k * k;
        for (idx_t i = shift; i <= N; ++i) {
            res[i] += dp[i - shift];
        }
    }

    seq_t ans = res[N];

    /* clean up */
    free(dp);
    free(dp_new);
    free(res);

    return ans;
}
