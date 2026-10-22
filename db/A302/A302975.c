/*
 * A302975  a(n) = denominator of tau(n)^n / n^tau(n).
 * https://oeis.org/A302975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of tau(n)^n / n^tau(n)
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 41, u64 n <= 167, u128 n <= 545,
 *            big any n
 * Helpers:   factor_u64, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

#define MAX_FACTS 64

seq_t a(idx_t n)
{
    if (n == 0) return 0;                         /* offset starts at 1 */
    uint64_t nn = (uint64_t)n;

    uint64_t primes[MAX_FACTS];
    uint64_t exps[MAX_FACTS];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    /* compute tau(n) */
    seq_t tau = 1;
    for (size_t i = 0; i < cnt; ++i)
        tau = seq_mul(tau, (seq_t)(exps[i] + 1));
    uint64_t tau64 = (uint64_t)tau;

    /* factor tau(n) */
    uint64_t tpr[MAX_FACTS];
    uint64_t tex[MAX_FACTS];
    size_t tcnt = 0;
    factor_u64(tau64, tpr, tex, &tcnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p      = primes[i];
        uint64_t e_n    = exps[i];
        uint64_t e_p_tau = 0;
        for (size_t j = 0; j < tcnt; ++j) {
            if (tpr[j] == p) { e_p_tau = tex[j]; break; }
        }

        __int128 exp_n_tau   = (__int128)e_n * tau64;
        __int128 exp_p_tau_n = (__int128)e_p_tau * nn;

        if (exp_p_tau_n >= exp_n_tau) continue;   /* denominator exponent 0 */
        uint64_t exp_den = (uint64_t)(exp_n_tau - exp_p_tau_n);

        seq_t pow_val = seq_pow((seq_t)p, (idx_t)exp_den);
        result = seq_mul(result, pow_val);
    }

    return result;
}
