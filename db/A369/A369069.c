/*
 * A369069  Dirichlet convolution of Liouville's lambda (A008836) with
 *          A083345, where A083345(n) = n' / gcd(n,n'), and n' stands for the
 *          arithmetic derivative of n, A003415.
 * https://oeis.org/A369069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} λ(n/d) * (d' / gcd(d,d'))
 * Range:     i8 n <= 63, i16 n <= 9826, i32 n <= 9826, i64 n <= 9826,
 *            i128 n <= 9826, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where λ is Liouville's λ function and d' is the arithmetic derivative of d */

#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

static uint64_t prime_factors[64];
static uint64_t exponents[64];
static uint64_t cur_exp[64];
static size_t fac_cnt;
static size_t sum_a;

/* recursive divisor enumeration */
static void dfs(size_t idx, seq_t d, size_t sum_b, seq_t *accum)
{
    if (idx == fac_cnt) {
        /* d is a divisor, cur_exp holds its exponents */
        seq_t dprime = 0;
        for (size_t j = 0; j < fac_cnt; ++j) {
            uint64_t bj = cur_exp[j];
            if (!bj) continue;
            seq_t term = d / (seq_t)prime_factors[j];
            term *= (seq_t)bj;
            dprime += term;
        }
        seq_t g = gcd(d, dprime);
        seq_t ratio = dprime / g;
        int sign = ((sum_a - sum_b) & 1) ? -1 : 1;
        if (sign > 0) *accum += ratio;
        else          *accum -= ratio;
        return;
    }

    seq_t d_val = d;
    for (uint64_t e = 0; e <= exponents[idx]; ++e) {
        cur_exp[idx] = e;
        dfs(idx + 1, d_val, sum_b + e, accum);
        d_val *= (seq_t)prime_factors[idx];
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1, but return 0 */

    /* factor n */
    factor_u64((uint64_t)n, prime_factors, exponents, &fac_cnt);
    sum_a = 0;
    for (size_t i = 0; i < fac_cnt; ++i) sum_a += exponents[i];
    /* clear current exponents array */
    for (size_t i = 0; i < fac_cnt; ++i) cur_exp[i] = 0;

    seq_t result = 0;
    dfs(0, (seq_t)1, 0, &result);
    return result;
}
