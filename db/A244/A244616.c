/*
 * A244616  G.f.: Sum_{n>=0} (3 + x^n)^n * x^n / (1-x)^(n+1).
 * https://oeis.org/A244616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A244616
   Formula: a(n) = 1 + Σ_{i=1}^{n} Σ_{k=0}^{min(i,⌊n/i⌋-1)} C(i,k) * 3^{i-k} * C(n - i*k, i)   (n ≥ 0) */
#include <stdlib.h>

/* multiplicative binomial coefficient
   Computes C(n,k) for 0 ≤ k ≤ n using 64‑bit or arbitrary precision arithmetic.
   k is reduced to min(k, n-k) to keep the number of iterations small. */
static seq_t binom_uint(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    /* Pre‑compute powers of 3 up to n: pow3[t] = 3^t */
    seq_t *pow3 = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    pow3[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        pow3[i] = pow3[i - 1] * (seq_t)3;

    seq_t result = (seq_t)1;               /* contribution of i = 0 term */

    for (idx_t i = 1; i <= n; ++i) {
        idx_t kmax = n / i;
        if (kmax == 0) continue;          /* i > n -> no contribution */
        kmax--;                            /* k must satisfy i*(k+1) ≤ n */
        if (kmax > i) kmax = i;            /* k cannot exceed i */

        seq_t binom_val = (seq_t)1;        /* C(i,0) */
        for (idx_t k = 0; k <= kmax; ++k) {
            idx_t hi = n - i * k;          /* hi = n - i*k  (≥ i) */
            seq_t comb = binom_uint(hi, i);/* C(hi, i) */
            seq_t term = binom_val;
            term = term * pow3[i - k];     /* * 3^(i-k) */
            term = term * comb;            /* * C(hi,i) */
            result = result + term;

            if (k < kmax) {               /* update C(i,k+1) = C(i,k)*(i-k)/(k+1) */
                seq_t numerator = binom_val * (seq_t)(i - k);
                binom_val = numerator / (seq_t)(k + 1);
            }
        }
    }

    free(pow3);
    return result;
}
