/*
 * A351926  a(n) = Sum_{d|n, p|n, (p^d)|n, p prime} p.
 * https://oeis.org/A351926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A351926:
   a(n) = \sum_{p\mid n} p \times \#\{\,d\mid n : d\le e_p\,\},
   where p^e_p is the exact power of prime p dividing n. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

static void _gen_divs(size_t idx, idx_t cur,
                      const uint64_t *primes, const uint64_t *exps,
                      size_t pc, idx_t *divs, size_t *pos)
{
    if (idx == pc) {
        divs[*pos] = cur;
        (*pos)++;
        return;
    }
    uint64_t p = primes[idx];
    idx_t prod = cur;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        _gen_divs(idx + 1, prod, primes, exps, pc, divs, pos);
        prod *= p;
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    /* Factor n into primes and exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Count total number of divisors. */
    size_t div_cnt = 1;
    for (size_t i = 0; i < cnt; ++i) {
        div_cnt *= (size_t)(exps[i] + 1);
    }

    /* Generate all divisors of n. */
    idx_t *divs = (idx_t *)malloc(div_cnt * sizeof(idx_t));
    size_t pos = 0;
    _gen_divs(0, 1, primes, exps, cnt, divs, &pos);

    /* Sum contributions per divisor. */
    seq_t sum = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        idx_t d = divs[i];
        for (size_t j = 0; j < cnt; ++j) {
            if (d <= exps[j]) {
                sum += (seq_t)primes[j];
            }
        }
    }

    free(divs);
    return sum;
}
