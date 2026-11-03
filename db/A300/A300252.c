/*
 * A300252  Difference between arithmetic derivative (A003415) and its Möbius
 *          transform (A300251).
 * https://oeis.org/A300252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415(n) - A300251(n) equivalent: a(n) = -sum_{d|n,
 *            d<n} μ(n/d) * A003415(d) where μ is the Möbius function
 * Range:     u8 n <= 191, u16 n <= 18431, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9024 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

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

/* Recursive generator of all divisors of n, accumulating
   contributions of each divisor d (d < n) to sumPos and sumNeg.
*/
static void dfs_div(size_t idx, idx_t curr, const idx_t *pr, const idx_t *ex,
                    int *ex_d, size_t cnt, idx_t n, seq_t *sumPos, seq_t *sumNeg)
{
    if (idx == cnt) {
        if (curr == n) return;          /* skip the divisor n itself */

        /* arithmetic derivative of curr */
        seq_t deriv = 0;
        for (size_t j = 0; j < cnt; ++j) {
            if (ex_d[j] == 0) continue;
            deriv += (seq_t)ex_d[j] * ((seq_t)curr / (seq_t)pr[j]);
        }

        /* Möbius of m = n / curr */
        int mu = 1;
        int parity = 0;
        for (size_t j = 0; j < cnt; ++j) {
            int e_m = (int)ex[j] - ex_d[j];
            if (e_m >= 2) { mu = 0; break; }
            if (e_m == 1) parity ^= 1;
        }
        if (mu == 0) return;
        mu = (parity == 0) ? 1 : -1;

        if (mu == 1) *sumPos += deriv;
        else          *sumNeg += deriv;
        return;
    }

    idx_t val = curr;
    for (int e = 0; e <= (int)ex[idx]; ++e) {
        ex_d[idx] = e;
        dfs_div(idx + 1, val, pr, ex, ex_d, cnt, n, sumPos, sumNeg);
        if (e < (int)ex[idx]) val *= pr[idx];
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    idx_t pr[64];
    idx_t ex[64];
    size_t cnt = 0;

    /* factor the number n */
    factor_u64((uint64_t)n, pr, ex, &cnt);

    int ex_d[64] = {0};

    seq_t sumPos = 0, sumNeg = 0;

    dfs_div(0, (idx_t)1, pr, ex, ex_d, cnt, n, &sumPos, &sumNeg);

    /* a(n) = sumNeg - sumPos  (sumNeg ≥ sumPos for all n) */
    seq_t result = sumNeg;
    result -= sumPos;
    return result;
}
