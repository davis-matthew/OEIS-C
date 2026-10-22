/*
 * A344206  Dirichlet g.f.: Product_{k>=2} 1 / (1 - k^(-s))^binomial(k+5,6).
 * https://oeis.org/A344206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   binom, binom_big_small, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A344206
   a(n) = sum over all unordered factorizations n = ∏ k_i^{e_i}
          of ∏ binom( binom(k_i+5,6) + e_i – 1 , e_i ),
   the so‑called Dirichlet coefficients of
   ∏_{k≥2} 1/(1-k^{-s})^{binom(k+5,6)}.
   This implementation uses recursion with the restriction
   that each integer k may be used at most once and the
   next integer k must be strictly larger than the one chosen
   before it. */

#include <stdint.h>
#include <stdlib.h>

#include "../utils/binom.h"      /* seq_t binom(idx_t a, idx_t b) */
#include "../utils/ensure_cache.h"
#include "../utils/binom_big_small.h"

/* forward declaration of recursive helper */
static seq_t rec(idx_t n, idx_t min_k);

/* integer square root (floor) using 128‑bit multiplication to avoid overflow */
static inline idx_t int_sqrt(idx_t n)
{
    idx_t r = 0;
    while (((unsigned __int128)(r + 1) * (r + 1)) <= n) ++r;
    return r;
}

/* recursive computation of a(n) with restriction that next k > previous k */
static seq_t rec(idx_t n, idx_t min_k)
{
    if (n == 1) return (seq_t)1;
    seq_t sum = (seq_t)0;

    /* collect all divisors of n that are >= min_k  (at most ~128) */
    idx_t divs[128];
    size_t div_cnt = 0;
    idx_t limit = int_sqrt(n);
    for (idx_t d = min_k; d <= limit; ++d) {
        if (n % d != 0) continue;
        divs[div_cnt++] = d;
        idx_t e = n / d;
        if (e != d && e >= min_k) divs[div_cnt++] = e;
    }
    if (n >= min_k) divs[div_cnt++] = n;   /* ensure n itself is present */

    for (size_t idx = 0; idx < div_cnt; ++idx) {
        idx_t k = divs[idx];
        if (k < min_k) continue;            /* safety */
        seq_t f_k = binom(k + 5, 6);        /* binom(k+5,6) */
        idx_t temp = n;
        idx_t e = 0;
        while (temp % k == 0) {
            temp /= k;
            ++e;
            seq_t term = binom_big_small(f_k + (seq_t)(e - 1), e);
            term = term * rec(temp, k + 1); /* next k must be larger than current k */
            sum += term;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* the sequence is defined for n ≥ 1; return 0 for n = 0 */
    if (n == 0) return (seq_t)0;
    return rec(n, 2);
}
