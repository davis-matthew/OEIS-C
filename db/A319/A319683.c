/*
 * A319683  Sum of A003415(d) over the proper divisors d of n, where A003415
 *          is arithmetic derivative.
 * https://oeis.org/A319683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d<n} A003415(d)
 * Range:     u8 n <= 95, u16 n <= 7679, u32 n <= 9788, u64 n <= 9788,
 *            u128 n <= 9788, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static void dfs_divs(size_t idx,
                     idx_t val,
                     const uint64_t *primes,
                     const uint64_t *exps,
                     size_t pcnt,
                     uint64_t *curExp,
                     seq_t *sum,
                     idx_t n)
{
    if (idx == pcnt) {
        if (val < n) {
            idx_t der = 0;
            for (size_t i = 0; i < pcnt; ++i) {
                if (curExp[i]) {
                    der += (idx_t)curExp[i] * (val / (idx_t)primes[i]);
                }
            }
            *sum += (seq_t)der;
        }
        return;
    }

    /* exponent 0 */
    curExp[idx] = 0;
    dfs_divs(idx + 1, val, primes, exps, pcnt, curExp, sum, n);

    /* exponents > 0 */
    idx_t newval = val;
    for (uint64_t e = 1; e <= exps[idx]; ++e) {
        newval *= (idx_t)primes[idx];
        curExp[idx] = e;
        dfs_divs(idx + 1, newval, primes, exps, pcnt, curExp, sum, n);
    }
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    uint64_t curExp[64] = {0};
    seq_t sum = 0;

    dfs_divs(0, (idx_t)1, primes, exps, pcnt, curExp, &sum, n);

    return sum;
}
