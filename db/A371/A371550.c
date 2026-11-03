/*
 * A371550  Expansion of e.g.f. Product_{k>=1} (1 + mu(k)^2*x^k/k!).
 * https://oeis.org/A371550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, for n>0 a(n) = Σ_{k squarefree≤n} binom(n,k)*a(n-k)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 28 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

seq_t a(idx_t n)
{
    /* dp[s] will hold a(s) for 0 ≤ s ≤ n */
    seq_t *dp = mem_new(seq_t, (size_t)n + 1);
    if (!dp) return 0;               /* allocation failure unlikely */
    dp[0] = 1;

    /* Gather all squarefree numbers up to n (mu(k) != 0) */
    idx_t *sq = NULL;
    size_t sq_cap = 0, sq_cnt = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (mu_of(k) != 0) {
            if (sq_cnt == sq_cap) {
                size_t new_cap = sq_cap ? sq_cap * 2 : 4;
                idx_t *tmp = (idx_t *)realloc(sq, new_cap * sizeof(idx_t));
                if (!tmp) { mem_free(dp); free(sq); return 0; }
                sq = tmp;
                sq_cap = new_cap;
            }
            sq[sq_cnt++] = k;
        }
    }

    /* Temporary array for binomial coefficients bin[s] = binom(s,k) */
    seq_t *bin = mem_new(seq_t, ((size_t)n + 1));
    if (!bin) { mem_free(dp); free(sq); return 0; }

    /* Process squarefree sizes in decreasing order to avoid repeated use of the same k */
    for (size_t idx = sq_cnt; idx-- > 0; ) {
        idx_t k = sq[idx];

        /* Compute binom(s,k) for s = k .. n using the recurrence
           binom(s,k) = binom(s-1,k) * s / (s-k) */
        bin[(size_t)k] = 1;                   /* binom(k,k) */
        for (size_t s = (size_t)k + 1; s <= (size_t)n; ++s) {
            bin[s] = bin[s - 1];
            bin[s] = bin[s] * (seq_t)s;       /* multiply first to keep division exact */
            bin[s] = bin[s] / (seq_t)(s - (size_t)k);
        }

        /* Update dp in decreasing order to avoid using updated values for the same k */
        for (size_t s = (size_t)n; s >= (size_t)k; --s) {
            seq_t add = bin[s] * dp[s - (size_t)k];
            dp[s] += add;
        }
    }

    seq_t result = dp[(size_t)n];

    mem_free(dp);
    free(sq);
    mem_free(bin);
    return result;
}
