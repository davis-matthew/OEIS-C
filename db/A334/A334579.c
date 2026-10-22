/*
 * A334579  a(n) = Sum_{d|n} gcd(tau(d), sigma(d)).
 * https://oeis.org/A334579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} gcd(tau(d), sigma(d))
 * Range:     u8 n <= 368, u16 n <= 7127, u32 n <= 7127, u64 n <= 7127,
 *            u128 n <= 7127, big any n
 * Helpers:   seq_gcd_builtin
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_gcd_builtin.h"

/* Recursive enumeration of all divisors of n given its factorisation. */
static void dfs_divisors(const uint64_t *pf, const int *exps, int pf_cnt,
                          int idx, seq_t cur_tau, seq_t cur_sigma, seq_t *sum) {
    if (idx == pf_cnt) {
        *sum += seq_gcd_builtin(cur_tau, cur_sigma);
        return;
    }
    uint64_t p = pf[idx];
    int maxe = exps[idx];
    seq_t tau_factor = 1;       /* (e+1) for current exponent e */
    idx_t  pow_p = 1;           /* p^e for current e */
    seq_t sigma_factor = 1;     /* 1 + p + p^2 + ... + p^e */

    for (int e = 0; e <= maxe; ++e) {
        seq_t new_tau = cur_tau * tau_factor;
        seq_t new_sigma = cur_sigma * sigma_factor;
        dfs_divisors(pf, exps, pf_cnt, idx + 1, new_tau, new_sigma, sum);

        if (e < maxe) {
            pow_p *= p;
            sigma_factor += pow_p;
            tau_factor++;     /* next exponent: (e+2) */
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;  /* value for undefined index, harmless */

    /* Factorisation of n by trial division (enough for n ≤ 10000). */
    uint64_t pf[16];
    int exps[16];
    int pf_cnt = 0;
    uint64_t tmp = n;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            do {
                tmp /= p;
                ++e;
            } while (tmp % p == 0);
            pf[pf_cnt] = p;
            exps[pf_cnt] = e;
            ++pf_cnt;
        }
    }
    if (tmp > 1) {          /* remaining prime factor */
        pf[pf_cnt] = tmp;
        exps[pf_cnt] = 1;
        ++pf_cnt;
    }

    seq_t sum = 0;
    dfs_divisors(pf, exps, pf_cnt, 0, 1, 1, &sum);
    return sum;
}
