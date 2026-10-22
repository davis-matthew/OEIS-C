/*
 * A305320  Expansion of Product_{k>=1} (1 - x^sigma(k)).
 * https://oeis.org/A305320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1}(1 - x^{σ(k)})
 * Range:     i8 n <= 168, i16 n <= 704, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where σ(k) is the sum-of-divisors function. */
/* The implementation computes all divisor sums up to n and then
   performs a standard subset‑sum convolution with factors (1 - x^e). */

#include <stdlib.h>

/* Helper to compute all σ(k) for k <= n */
static void compute_sigma(idx_t n, idx_t *sigma)
{
    for (idx_t i = 1; i <= n; ++i)
        for (idx_t j = i; j <= n; j += i)
            sigma[j] += i;
}

seq_t a(idx_t n)
{
    /* allocate arrays */
    idx_t *sigma = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!sigma) return 0;  /* out of memory, caller must handle */
    for (idx_t i = 0; i <= n; ++i) sigma[i] = 0;

    /* compute divisor sums */
    compute_sigma(n, sigma);

    seq_t *coef = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!coef) { free(sigma); return 0; }
    for (idx_t i = 0; i <= n; ++i) coef[i] = 0;
    coef[0] = 1;

    /* multiply by each factor (1 - x^e) */
    for (idx_t k = 1; k <= n; ++k) {
        idx_t e = sigma[k];
        if (e > n) continue;          /* factor irrelevant for this n */
        /* inner loop: for every j from 0..n-e, subtract coef[j] from coef[j+e] */
        for (long long j = (long long)n - (long long)e; j >= 0; --j) {
            idx_t idx = (idx_t)(j + e);
            coef[idx] -= coef[(idx_t)j];
        }
    }

    seq_t result = coef[n];

    free(sigma);
    free(coef);
    return result;
}
