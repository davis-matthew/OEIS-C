/*
 * A324987  a(n) = Product_{d|n} (tau(d)*sigma(d)) where tau(k) = the number
 *          of divisors of k (A000005) and sigma(k) = the sum of the divisors
 *          of k (A000203).
 * https://oeis.org/A324987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n} tau(d)*sigma(d)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

static void _dfs(int idx, int pfcnt, const uint64_t *pr,
                 const int *exp, seq_t cur_tau,
                 seq_t cur_sigma, seq_t *res)
{
    if (idx == pfcnt) {
        *res *= cur_tau * cur_sigma;
        return;
    }
    uint64_t p = pr[idx];
    int emax = exp[idx];
    for (int e = 0; e <= emax; ++e) {
        seq_t sigma_term = 1;
        if (e != 0) {
            seq_t pow_val = pow_uint(p, (idx_t)(e + 1));
            seq_t denom = (seq_t)(p - 1);
            sigma_term = (pow_val - 1) / denom;
        }
        seq_t next_tau = cur_tau * (seq_t)(e + 1);
        seq_t next_sigma = cur_sigma * sigma_term;
        _dfs(idx + 1, pfcnt, pr, exp, next_tau, next_sigma, res);
    }
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t temp = (uint64_t)n;
    uint64_t primes[64];
    int exps[64];
    int pfcnt = 0;

    uint64_t t = temp;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            primes[pfcnt] = p;
            exps[pfcnt] = e;
            ++pfcnt;
        }
    }
    if (t > 1) {
        primes[pfcnt] = t;
        exps[pfcnt] = 1;
        ++pfcnt;
    }

    _dfs(0, pfcnt, primes, exps, 1, 1, &result);
    return result;
}
