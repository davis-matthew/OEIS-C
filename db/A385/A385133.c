/*
 * A385133  The number of integers k from 1 to n such that gcd(n, k) has an
 *          odd maximum exponent in its prime factorization.
 * https://oeis.org/A385133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} (A051903(gcd(n,k)) mod 2)
 * Range:     u8 n <= 389, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation by: mertenslaw: compute using divisor enumeration and Euler totient. */
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Recursive helper that enumerates all divisors of n. */
static void dfs_div(idx_t idx,
                    uint64_t cur_val,
                    int cur_max,
                    const uint64_t *primes,
                    const int *exp,
                    size_t cnt,
                    uint64_t N,
                    seq_t *sum,
                    int *chosen)
{
    if (idx == cnt) {
        seq_t phi_m = (seq_t)1;
        for (size_t j = 0; j < cnt; ++j) {
            int g = exp[j] - chosen[j];
            if (g > 0) {
                seq_t t = (seq_t)1;
                for (int k = 0; k < g - 1; ++k) t *= (seq_t)primes[j];
                t *= (seq_t)(primes[j] - 1);
                phi_m *= t;
            }
        }
        if (cur_max & 1) *sum += phi_m;
        return;
    }
    const uint64_t p     = primes[idx];
    const int    e       = exp[idx];
    uint64_t new_val = cur_val;
    for (int exp_val = 0; exp_val <= e; ++exp_val) {
        chosen[idx] = exp_val;
        dfs_div(idx + 1,
                new_val,
                (exp_val > cur_max ? exp_val : cur_max),
                primes,
                exp,
                cnt,
                N,
                sum,
                chosen);
        if (exp_val < e) new_val = (__uint128_t)new_val * p;
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    uint64_t N = (uint64_t)n;
    uint64_t primes_u[16];
    uint64_t exps_u[16];
    size_t cnt = 0;
    factor_u64(N, primes_u, exps_u, &cnt);

    int primes[16];
    int exps[16];
    for (size_t i = 0; i < cnt; ++i) {
        primes[i] = (int)primes_u[i];
        exps[i]   = (int)exps_u[i];
    }

    int chosen[16] = {0};
    seq_t sum      = (seq_t)0;
    dfs_div(0,
            (uint64_t)1,
            0,
            primes_u,
            exps,
            cnt,
            N,
            &sum,
            chosen);
    return sum;
}
