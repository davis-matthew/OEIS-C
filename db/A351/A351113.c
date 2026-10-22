/*
 * A351113  Sum of the balanced numbers dividing n.
 * https://oeis.org/A351113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, phi(d)|sigma(d)} d (balanced divisors of n)
 * Range:     u8 n <= 22, u16 n <= 9320, u32 n <= 9320, u64 n <= 9320,
 *            u128 n <= 9320, big any n
 * Helpers:   factor_u64, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_int.h"

static void dfs_factor(const uint64_t *pr, const uint64_t *ex, size_t len,
                       size_t idx, seq_t d, seq_t phi, seq_t sigma,
                       seq_t *sum)
{
    if (idx == len) {
        if (sigma % phi == 0)
            *sum += d;
        return;
    }
    uint64_t p = pr[idx];
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        /* p^e  */
        seq_t powp = pow_int((seq_t)p, (idx_t)e);
        seq_t new_d = d * powp;

        /* φ(p^e) */
        seq_t phi_pe = 1;
        if (e > 0) {
            seq_t powp_e = pow_int((seq_t)p, (idx_t)e);
            seq_t powp_e_1 = pow_int((seq_t)p, (idx_t)(e - 1));
            phi_pe = powp_e - powp_e_1;
        }

        /* σ(p^e) */
        seq_t sigma_pe = 1;
        if (e > 0) {
            seq_t powp_e1 = pow_int((seq_t)p, (idx_t)(e + 1));
            sigma_pe = (powp_e1 - 1) / (seq_t)(p - 1);
        }

        seq_t new_phi = phi * phi_pe;
        seq_t new_sigma = sigma * sigma_pe;

        dfs_factor(pr, ex, len, idx + 1, new_d, new_phi, new_sigma, sum);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety for out‑of‑bounds */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    dfs_factor(primes, exps, cnt, 0, (seq_t)1, (seq_t)1, (seq_t)1, &sum);
    return sum;
}
