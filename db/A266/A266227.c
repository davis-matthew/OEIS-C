/*
 * A266227  a(n) = floor(Sum_{d|n} 1/sigma(d)).
 * https://oeis.org/A266227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( Σ_{d|n} 1/σ(d) ) where σ is the sum of divisors
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 4959, u64 n <= 8676,
 *            u128 n <= 8676, big any n
 * Helpers:   factor_u64, seq_gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/seq_gcd.h"
#include "../utils/seq_pow.h"

/* Recursive helper that enumerates all divisors of n.
   curr_sigma holds σ(d) for the divisor constructed so far. */
static void dfs(const uint64_t *primes,
                const uint64_t *exps,
                size_t        idx,
                size_t        cnt,
                seq_t         curr_sigma,
                seq_t        *sum_num,
                seq_t        *sum_den)
{
    if (idx == cnt) {                 /* reached a divisor – add 1/σ(d) */
        seq_t num = (*sum_num) * curr_sigma + (*sum_den);
        seq_t den = (*sum_den) * curr_sigma;
        seq_t g   = seq_gcd(num, den);
        if (g != 1) {                 /* reduce the fraction */
            num /= g;
            den /= g;
        }
        *sum_num = num;
        *sum_den = den;
        return;
    }

    uint64_t p   = primes[idx];
    uint64_t emax = exps[idx];

    /* Pre‑compute σ(p^e) for e = 0 … emax (emax < 64 for 64‑bit n) */
    seq_t sigma_table[64];
    sigma_table[0] = 1;
    for (uint64_t e = 1; e <= emax; ++e) {
        seq_t powp = seq_pow((seq_t)p, (idx_t)(e + 1));
        sigma_table[e] = (powp - 1) / ((seq_t)p - 1);
    }

    for (uint64_t e = 0; e <= emax; ++e) {
        dfs(primes, exps, idx + 1, cnt,
            curr_sigma * sigma_table[e], sum_num, sum_den);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* defined only for n ≥ 1 */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    /* factor_u64 expects uint64_t arguments; cast n accordingly */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum_num = 0;
    seq_t sum_den = 1;

    dfs(primes, exps, 0, cnt, 1, &sum_num, &sum_den);

    return sum_num / sum_den;        /* floor of the rational sum */
}
