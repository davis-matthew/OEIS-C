/*
 * A265627  Number of n X n "primitive" binary matrices.
 * https://oeis.org/A265627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n} sum_{e|n} mu(d)*mu(e)*2^(n^2/(d*e))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primitive binary n×n matrices: number of matrices that cannot be tiled
 * by a smaller matrix.  The implementation follows the OEIS formula
 * using trial division to collect the divisors of n and the Möbius
 * function computed from factor_u64.  Results are computed on every
 * call; no dynamic memory is allocated, so the code works for both
 * plain integer and arbitrary‑precision seq_t types. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

static inline void get_divisors(idx_t n, idx_t *divs, size_t *cnt)
{
    size_t c = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            divs[c++] = d;
            if (d != n / d) divs[c++] = n / d;
        }
    }
    *cnt = c;
    /* quick deterministic order (optional) */
    for (size_t i = 0; i < c; ++i)
        for (size_t j = i + 1; j < c; ++j)
            if (divs[i] > divs[j]) {
                idx_t t = divs[i];
                divs[i] = divs[j];
                divs[j] = t;
            }
}

/* Möbius function from prime factorisation */
static int mobius(idx_t n)
{
    if (n == 1) return 1;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] > 1) return 0;          /* squared prime factor */
    return (cnt % 2 == 0) ? 1 : -1;         /* (-1)^k */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset 1 */
    seq_t result = 0;
    seq_t n_sq = ((seq_t)n) * ((seq_t)n);

    idx_t divs[1024];
    size_t d_cnt;
    get_divisors(n, divs, &d_cnt);

    seq_t pos = 0;
    seq_t neg = 0;

    for (size_t i = 0; i < d_cnt; ++i) {
        idx_t d = divs[i];
        int mu_d = mobius(d);
        if (mu_d == 0) continue;
        for (size_t j = 0; j < d_cnt; ++j) {
            idx_t e = divs[j];
            int mu_e = mobius(e);
            if (mu_e == 0) continue;
            seq_t de = ((seq_t)d) * ((seq_t)e);
            seq_t exp = n_sq / de;
            seq_t term = big_pow((seq_t)2, exp);
            if (mu_d * mu_e > 0)
                pos += term;
            else
                neg += term;
        }
    }
    result = pos - neg;   /* pos >= neg because the final result is positive */
    return result;
}
