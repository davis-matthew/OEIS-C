/*
 * A343324  Dirichlet g.f.: Product_{k>=2} (1 + k^(-s))^(k^4).
 * https://oeis.org/A343324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{n = ∏ k_i^{e_i} (k_i ≥ 2, e_i ≥ 1)} ∏ binom(k_i^4,
 *            e_i) (coefficients taken with exact integer arithmetic)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Compute binomial coefficient binom(N, e) for small e using only   */
/* seq_t arithmetic.  e is always ≤ log_k(n) (≤ 14 for n ≤ 10000).   */
/* ------------------------------------------------------------------ */
static seq_t binom_small(seq_t N, unsigned e)
{
    if (e == 0U)
        return (seq_t)1;
    seq_t res = (seq_t)1;
    /* multiplicative formula: res = ∏_{i=1..e} (N - e + i) / i */
    for (unsigned i = 1U; i <= e; ++i) {
        res *= (N - (seq_t)e + (seq_t)i);
        res /= (seq_t)i;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Recursive helper: sum over factorizations of `n` using factors     */
/* ≥ min_factor.  `min_factor` ensures a canonical ordering.          */
/* ------------------------------------------------------------------ */
static seq_t rec(idx_t n, idx_t min_factor)
{
    if (n == (idx_t)1)
        return (seq_t)1;  /* empty factorization */

    seq_t sum = (seq_t)0;

    /* collect all divisors k of n with k ≥ min_factor */
    idx_t divisors[64];
    unsigned cnt = 0U;
    for (idx_t d = min_factor; d * d <= n; ++d) {
        if (n % d == (idx_t)0) {
            divisors[cnt++] = d;
            idx_t e = n / d;
            if (e != d)
                divisors[cnt++] = e;
        }
    }
    /* include n itself as a single factor (k = n) */
    if (n >= min_factor)
        divisors[cnt++] = n;

    for (unsigned t = 0U; t < cnt; ++t) {
        idx_t k = divisors[t];
        if (k < min_factor)            /* safety check */
            continue;

        /* compute k^4 as seq_t (fits in 64 bits for k ≤ 10000) */
        seq_t k2 = (seq_t)k * (seq_t)k;
        seq_t k4 = k2 * k2;

        /* try all admissible exponents e for this factor   */
        idx_t powk = k;
        unsigned e   = 1U;
        while (powk != 0 && n % powk == (idx_t)0) {
            idx_t rest = n / powk;
            seq_t weight = binom_small(k4, e);
            seq_t sub    = rec(rest, k + (idx_t)1);
            sum += weight * sub;

            /* avoid overflow of powk */
            if (powk > n / k)
                break;
            powk *= k;
            ++e;
        }
    }

    return sum;
}

/* ------------------------------------------------------------------ */
/* Public entry point for the sequence                                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;            /* undefined for n < 1 */
    return rec(n, (idx_t)2);
}
