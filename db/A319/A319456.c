/*
 * A319456  a(n) = [x^n] Product_{k>=1} ((1 - x^k)*(1 - x^(2*k)))^n.
 * https://oeis.org/A319456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 19, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS formula: a(n) = coefficient of x^n in  
 *   ∏_{k>=1} ((1 − x^k)(1 − x^(2k)))^n
 *
 * Recurrence derived from the exponential generating function:
 *   a(0) = 1
 *   a(n) = (n/n) * Σ_{k=1}^n (σ(2k)−4σ(k)) * a(n−k) ,
 *   that is
 *   a(n) = Σ_{k=1}^n (σ(2k)−4σ(k)) * a(n−k)  * (n / n)
 *   →  a(n) = (Σ_{k=1}^n (σ(2k)−4σ(k)) * a(n−k)) * n / n
 *   which simplifies to
 *   a(n) = (Σ_{k=1}^n weight(k) * a(n−k)) * n / n
 *   with weight(k) = σ(2k)−4σ(k).
 *
 * Implementation uses a double loop over 1..n for
 *   - computing σ(k) for k≤2n
 *   - computing a(n) by dynamic programming.
 */

#include <stdlib.h>   /* malloc, free */
#include <stdint.h>   /* size_t */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    /* ---------- compute divisor sums σ(k) for 1..2n ---------- */
    idx_t limit = 2 * n;
    seq_t *sigma = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    if (!sigma) return (seq_t)0;   /* allocation fail -> zero term */
    for (idx_t i = 0; i <= limit; ++i)
        sigma[i] = (seq_t)0;

    for (idx_t i = 1; i <= limit; ++i) {
        seq_t add = (seq_t)i;
        for (idx_t j = i; j <= limit; j += i)
            sigma[j] += add;
    }

    /* ---------- precompute weights weight(k) = σ(2k)−4σ(k) ---------- */
    seq_t *weight = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!weight) { free(sigma); return (seq_t)0; }
    weight[0] = (seq_t)0;   /* unused */
    for (idx_t k = 1; k <= n; ++k) {
        weight[k] = sigma[2 * k] - (seq_t)4 * sigma[k];
    }

    /* ---------- dynamic programming for a(i), 0..n ---------- */
    seq_t *a_val = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!a_val) { free(sigma); free(weight); return (seq_t)0; }
    a_val[0] = (seq_t)1;

    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k) {
            sum += weight[k] * a_val[m - k];
        }
        /* multiplication by n then division by m (exact) */
        sum *= (seq_t)n;
        a_val[m] = sum / (seq_t)m;
    }

    seq_t result = a_val[n];

    free(sigma);
    free(weight);
    free(a_val);
    return result;
}
