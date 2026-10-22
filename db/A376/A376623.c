/*
 * A376623  G.f.: Sum_{k>=0} x^(k*(k+1)) * Product_{j=1..k} (1 + x^(2*j-1))^2.
 * https://oeis.org/A376623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in Sum_{k>=0} x^(k*(k+1)) *
 *            Prod_{j=1..k} (1 + x^(2*j-1))^2. For each k, the product expands
 *            as the convolution of 1 + 2*x^(2k-1) + x^(4k-2)
 * Range:     u8 n <= 79, u16 n <= 223, u32 n <= 700, u64 n <= 2399,
 *            u128 n <= 8734, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    size_t N = (size_t)n + 1ULL;
    seq_t *res = NULL;
    seq_t *dp  = NULL;
    seq_t *newdp = NULL;

#ifdef __cplusplus
    /* In C++ (big‑integer build) use new[] so the objects are
       properly constructed.  Afterwards we set them to 0. */
    res   = new seq_t[N];
    dp    = new seq_t[N];
    newdp = new seq_t[N];
    for (size_t i = 0; i < N; ++i) {
        res[i]   = 0;
        dp[i]    = 0;
        newdp[i] = 0;
    }
#else
    /* In plain C (fixed‑size integer build) use calloc. */
    res   = (seq_t*)calloc(N, sizeof(seq_t));
    dp    = (seq_t*)calloc(N, sizeof(seq_t));
    newdp = (seq_t*)calloc(N, sizeof(seq_t));
#endif
    if (!res || !dp || !newdp) {
#ifdef __cplusplus
        delete[] res;
        delete[] dp;
        delete[] newdp;
#else
        free(res);
        free(dp);
        free(newdp);
#endif
        return (seq_t)0;
    }

    dp[0] = 1;   /* polynomial for k = 0 */

    /* compute the largest k with k(k+1) <= n */
    idx_t Kmax = 0;
    while ((Kmax + 1ULL) * (Kmax + 2ULL) <= n) ++Kmax;

    for (idx_t k = 1; k <= Kmax; ++k) {
        /* clear newdp for the new iteration */
        for (size_t i = 0; i < N; ++i) newdp[i] = 0;

        idx_t a = 2ULL * k - 1ULL;     /* shift 2k-1 */
        idx_t b = 2ULL * a;            /* shift 4k-2 */

        for (size_t t = 0; t < N; ++t) {
            seq_t val = dp[t];
            if (!val) continue;

            /* term 0 */
            newdp[t] += val;

            /* term 2*x^a */
            if (t + a < N)
                newdp[t + a] += val * (seq_t)2;

            /* term x^b */
            if (t + b < N)
                newdp[t + b] += val;
        }

        /* add contribution of this k to the result */
        idx_t base = k * (k + 1);
        if (base < N) {
            for (size_t t = 0; t + base < N; ++t) {
                if (newdp[t]) res[base + t] += newdp[t];
            }
        }

        /* prepare for next iteration */
        seq_t *tmp = dp;
        dp        = newdp;
        newdp     = tmp;
    }

    /* add the k = 0 term (x^0 * 1) */
    res[0] += 1;

    seq_t ans = res[n];

#ifdef __cplusplus
    delete[] res;
    delete[] dp;
    delete[] newdp;
#else
    free(res);
    free(dp);
    free(newdp);
#endif
    return ans;
}
