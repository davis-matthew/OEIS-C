/*
 * A305654  a(n) = [x^n] exp(Sum_{k>=1} x^k*(1 + x^k)/(k*(1 - x^k)^n)).
 * https://oeis.org/A305654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] ∏_{k≥1} 1/(1–x^k)^{2·binomial(n+k–2,n–1) –
 *            binomial(n+k–3,n–2)}
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/binomial.h"

/* Compute a(n), n ≥ 0.  For n = 0 the value is 1. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    idx_t size = n + 1;
    seq_t *dp     = (seq_t *)malloc(size * sizeof(seq_t));
    seq_t *newdp  = (seq_t *)malloc(size * sizeof(seq_t));
    if (!dp || !newdp) {
        return (seq_t)0;
    }

    /* dp[0] = 1, others 0 */
    for (idx_t i = 0; i < size; ++i) {
        dp[i] = (seq_t)0;
    }
    dp[0] = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k) {
        /* exponent e(k) = 2·C(n+k-2,n-1) – C(n+k-3,n-2) */
        seq_t bin1 = binomial((idx_t)(n + k - 2), n - 1);

        seq_t bin2;
        /* Handle the special case when n==1; then n-2==-1.  For k==1
           we need C(-1,-1)=1, otherwise the binomial is 0. */
        if (n >= 2) {
            bin2 = binomial((idx_t)(n + k - 3), n - 2);
        } else if (n == 1 && k == 1) {
            bin2 = (seq_t)1;
        } else {
            bin2 = (seq_t)0;
        }

        seq_t e = bin1 + bin1 - bin2;       /* may be large, but kept in seq_t */

        idx_t tmax = n / k;  /* maximum number of parts of size k */

        /* zero newdp */
        for (idx_t i = 0; i < size; ++i) {
            newdp[i] = (seq_t)0;
        }

        seq_t coeff_t = (seq_t)1;   /* binom(e + t – 1, t) for the current t */

        for (idx_t t = 0; t <= tmax; ++t) {
            idx_t shift = t * k;

            /* add contributions of t parts of size k */
            for (idx_t j = 0; j + shift <= n; ++j) {
                newdp[j + shift] += dp[j] * coeff_t;
            }

            /* prepare coeff_t for next t (if any) */
            if (t < tmax) {
                /* coeff_t = coeff_t * (e + t) / (t + 1) */
                seq_t numerator = coeff_t * (e + (seq_t)t);
                coeff_t = numerator / (seq_t)(t + 1);
            }
        }

        /* swap dp and newdp for next iteration */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    seq_t answer = dp[n];

    free(dp);
    free(newdp);

    return answer;
}
