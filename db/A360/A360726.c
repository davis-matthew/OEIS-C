/*
 * A360726  Expansion of Sum_{k>0} (k * x * (1 + x^k))^k.
 * https://oeis.org/A360726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^d * binomial(d, n/d-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, binomial, factor_u64, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
/* Utility function prototypes */
#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/factor_u64.h"
#include "../utils/seq_mul.h"

/* Recursive divisor enumeration and term accumulation */
static void gen_divisors(const uint64_t *primes,
                         const uint64_t *exps,
                         size_t      ntf,
                         size_t      idx,
                         idx_t       cur,
                         idx_t       n,
                         seq_t      *sum)
{
    if (idx == ntf) {
        seq_t bin = binomial((idx_t)cur, (idx_t)(n / cur - 1));
        seq_t pow = big_pow((seq_t)cur, (seq_t)cur);
        seq_t term = seq_mul(bin, pow);
        *sum += term;
        return;
    }

    idx_t val = cur;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        gen_divisors(primes, exps, ntf, idx + 1, val, n, sum);
        if (e < exps[idx]) val *= primes[idx];
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Factorize n into prime powers */
    uint64_t pf[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64(n, pf, exps, &cnt);

    /* Sum over all divisors */
    seq_t total = 0;
    gen_divisors(pf, exps, cnt, 0, 1, n, &total);
    return total;
}
