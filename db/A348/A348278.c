/*
 * A348278  a(n) = Sum_{d|n} d^(d').
 * https://oeis.org/A348278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(d') where d' = Σ_{p^e||d} e * d / p
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

static void rec(const uint64_t *pr, const uint64_t *ex,
                size_t cnt, size_t idx, uint64_t d,
                uint64_t *chosen, seq_t *res)
{
    if (idx == cnt) {
        seq_t d_prime = 0;
        for (size_t i = 0; i < cnt; i++) {
            if (chosen[i]) {
                /* term = chosen[i] * (d / pr[i]) */
                d_prime += (seq_t)chosen[i] * (seq_t)(d / pr[i]);
            }
        }
        seq_t term = big_pow((seq_t)d, d_prime);
        *res += term;
        return;
    }

    uint64_t p = pr[idx];
    uint64_t p_pow = 1;
    for (uint64_t e = 0; e <= ex[idx]; e++) {
        chosen[idx] = e;
        rec(pr, ex, cnt, idx + 1, d * p_pow, chosen, res);
        p_pow *= p;
    }
    chosen[idx] = 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* n >= 1 by OEIS offset, but guard anyway. */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 0;
    uint64_t chosen[64] = {0};

    rec(primes, exps, cnt, 0, 1ULL, chosen, &result);
    return result;
}
