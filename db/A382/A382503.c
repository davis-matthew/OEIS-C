/*
 * A382503  a(n) = Sum_{d|n} binomial(2*d-1,d).
 * https://oeis.org/A382503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} binomial(2*d-1,d) (OEIS A382503)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k); */
#include "../utils/factor_u64.h" /* provides void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

/* Recursively generate all divisors of n from its prime factorization. */
static void
gen_divs(const uint64_t *pr, const uint64_t *ex, size_t cnt,
         size_t depth, uint64_t cur, uint64_t *divs, size_t *idx)
{
    if (depth == cnt) {
        divs[(*idx)++] = cur;
        return;
    }
    uint64_t p = pr[depth];
    uint64_t val = cur;
    for (uint64_t e = 0; e <= ex[depth]; ++e) {
        gen_divs(pr, ex, cnt, depth + 1, val, divs, idx);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* Undefined for n = 0, harmless */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t prime_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &prime_cnt);

    /* Compute the number of divisors: product of (e_i + 1). */
    size_t div_count = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        div_count *= (size_t)(exps[i] + 1);
    }

    uint64_t *divs = (uint64_t *)malloc(div_count * sizeof(uint64_t));
    if (!divs) return 0; /* Allocation failure, return 0 as fallback */

    size_t didx = 0;
    gen_divs(primes, exps, prime_cnt, 0, 1ULL, divs, &didx);

    seq_t sum = 0;
    for (size_t i = 0; i < didx; ++i) {
        uint64_t d = divs[i];
        idx_t nl = (idx_t)(d * 2ULL - 1ULL);
        sum += binomial(nl, (idx_t)d);
    }

    free(divs);
    return sum;
}
