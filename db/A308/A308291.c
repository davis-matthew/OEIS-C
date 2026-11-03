/*
 * A308291  Expansion of Sum_{k>=1} mu(k)*log(1 + x^k/(1 - x^k)^4)/k.
 * https://oeis.org/A308291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the inverse Euler transform of the tetrahedral numbers *
 *            (A000292). The sequence satisfies * -1 + Product_{m>=1} 1/(1 -
 *            x^m)^a(m) = 1 + Sum_{m>=1} binomial(m+2,3) x^m . * The
 *            computation uses the relations * S_n = Σ_{d|n} a(d) * d (1) *
 *            S_n = n * B_n - Σ_{k=1}^{n-1} S_k * B_{n-k} (2) * wh...
 * Range:     i8 n <= 5, i16 n <= 14, i32 n <= 30, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
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

seq_t a(idx_t n) {
    /* static cache for S[1..max_n] and B[1..max_n] */
    static seq_t *S = NULL;
    static seq_t *B = NULL;
    static idx_t max_n = 0;

    if (n > max_n) {
        /* expand arrays */
        size_t newsize = n + 1;                    /* 1‑based arrays */
        S = mem_resize(seq_t, S, newsize);
        B = mem_resize(seq_t, B, newsize);

        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t it   = (seq_t)i;
            seq_t bi   = it * (it + 1) * (it + 2) / 6;   /* tetrahedral number */
            B[i] = bi;

            if (i == 1) {                  /* initial value of S_1 */
                S[1] = bi;                  /* S_1 = B_1 = 1 */
            } else {
                seq_t sum = 0;
                for (idx_t k = 1; k < i; ++k) {
                    sum += S[k] * B[i - k];
                }
                /* from (2):  S_i = i*B_i - Σ_{k<i} S_k*B_{i-k} */
                S[i] = it * bi - sum;
            }
        }
        max_n = n;
    }

    /* compute a(n) using (3) */
    seq_t sum_mu = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;
            /* first divisor d1 */
            sum_mu += (seq_t)mu_of(d1) * S[d2];
            if (d1 != d2) {  /* paired divisor d2 */
                sum_mu += (seq_t)mu_of(d2) * S[d1];
            }
        }
    }
    return sum_mu / (seq_t)n;
}
