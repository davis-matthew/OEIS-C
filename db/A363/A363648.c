/*
 * A363648  Expansion of Sum_{k>0} (1/(1 - (k*x)^k)^4 - 1).
 * https://oeis.org/A363648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d)^n * binomial(d+3,3)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/binomial.h"    /* binomial(idx_t a, idx_t b) */
#include "../utils/big_pow.h"     /* big_pow(seq_t base, seq_t exp) */

/* generate all divisors of n given its prime factorisation */
static void gen_divisors(const uint64_t *pf, const uint64_t *exp, size_t cnt,
                         idx_t depth, idx_t current, idx_t *divs, size_t *len)
{
    if (depth == cnt) {
        divs[(*len)++] = current;
        return;
    }
    uint64_t p = pf[depth];
    uint64_t e = exp[depth];
    idx_t prod = current;
    for (uint64_t i = 0; i <= e; ++i) {
        gen_divisors(pf, exp, cnt, depth + 1, prod, divs, len);
        prod *= p;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* safety for off‑offset calls */

    /* factorise n */
    uint64_t pf[64];
    uint64_t exp[64];
    size_t pf_cnt;
    factor_u64((uint64_t)n, pf, exp, &pf_cnt);

    /* generate all divisors of n */
    idx_t divs[512];
    size_t divlen = 0;
    gen_divisors(pf, exp, pf_cnt, 0, (idx_t)1, divs, &divlen);

    /* sum over divisors */
    seq_t result = 0;
    for (size_t i = 0; i < divlen; ++i) {
        idx_t d = divs[i];
        seq_t base = (seq_t)(n / d);                     /* (n/d) */
        seq_t powd = big_pow(base, (seq_t)n);            /* (n/d)^n */
        seq_t bin = binomial(d + 3, 3);                  /* binomial(d+3,3) */
        result += powd * bin;
    }
    return result;
}
