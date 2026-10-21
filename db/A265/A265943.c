/*
 * A265943  E.g.f.: Sum_{n>=1} x^(n^2) * exp(3*x^n) / n!.
 * https://oeis.org/A265943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 3^(d-n/d) * binomial(d, n/d) * n!/d! (n ≥ 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 20, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Multiplicative binomial coefficient: C(n, k) */
static seq_t binomial_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;     /* division is exact */
    }
    return res;
}

/* Power 3^exp, exp is non‑negative */
static seq_t pow3_idx(idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) res *= 3;
    return res;
}

/* Return the sum a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; return 0 for safety */

    seq_t res = 0;

    /* Enumerate divisor pairs (i, n/i) for i ≤ sqrt(n) */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i != 0) continue;

        idx_t d_small = i;
        idx_t d_large = n / i;     /* larger divisor in the pair */

        /* Use the larger divisor d and the smaller m = n/d for the term */
        idx_t d = d_large;
        idx_t m = d_small;

        /* term is defined only for d ≥ m (ensured by construction) */
        idx_t exp = d - m;
        seq_t pow3   = pow3_idx(exp);
        seq_t bin    = binomial_idx(d, m);

        /* n! / d! = product_{k=d+1}^{n} k (empty product equals 1 if d == n) */
        seq_t prod_dn = 1;
        for (idx_t k = d + 1; k <= n; ++k) prod_dn *= (seq_t)k;

        seq_t term = prod_dn;
        term = term * bin;
        term = term * pow3;

        res += term;
    }

    return res;
}
