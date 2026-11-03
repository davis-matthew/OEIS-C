/*
 * A322672  a(n) = Product_{d|n} (pod(d)/d) where pod(k) is the product of the
 *          divisors of k (A007955).
 * https://oeis.org/A322672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n} (pod(d)/d) where pod(k) = For all d|k:
 *            Product(d)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 35, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-01)
 * Helpers:   factor_u64, ipow_i32, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_i32.h"
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Recursively generate all divisors of n from its prime factorization,
   accumulating the product of pod(d)/d in *result. */
static void
gen_divisors(const uint64_t *pr, const uint64_t *ex, size_t pf_cnt, size_t depth,
             seq_t cur_d, idx_t cur_tau, seq_t *result)
{
    if (depth == pf_cnt) {
        /* Skip the divisor d = 1; pod(1)/1 = 1 by definition. */
        if (cur_d == (seq_t)1) return;

        seq_t term;
        if (cur_tau & 1) {              /* τ(d) is odd → d is a perfect square */
            seq_t sqrt_d = floor_sqrt(cur_d);   /* sqrt(d) is integer */
            idx_t exp = cur_tau - 2;            /* τ-2 ≥ 0 for τ≥3 */
            term = ipow_idx(sqrt_d, exp);
        } else {                         /* τ(d) is even → normal case */
            idx_t exp = (cur_tau >> 1) - 1;     /* τ/2 - 1 */
            term = ipow_idx(cur_d, exp);
        }
        *result = seq_mul(*result, term);
        return;
    }

    /* For prime pr[depth], exponent ranges from 0 .. ex[depth] */
    uint64_t p     = pr[depth];
    size_t  max_e  = (size_t)ex[depth];
    seq_t   p_pow  = (seq_t)1; /* p^0 */

    for (size_t e = 0; e <= max_e; ++e) {
        seq_t next_d = seq_mul(cur_d, p_pow);
        idx_t next_tau = cur_tau * (idx_t)(e + 1);
        gen_divisors(pr, ex, pf_cnt, depth + 1, next_d, next_tau, result);

        /* Prepare p^(e+1) for the next iteration. */
        if (e < max_e) {
            p_pow = seq_mul(p_pow, (seq_t)p);
        }
    }
}

/* Public function required by the sequence specification. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;  /* undefined but keep sanity */

    uint64_t pr[64], ex[64];
    size_t pf_cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &pf_cnt);

    seq_t result = (seq_t)1;
    gen_divisors(pr, ex, pf_cnt, 0, (seq_t)1, (idx_t)1, &result);
    return result;
}
