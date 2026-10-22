/*
 * A324984  a(n) = numerator of Sum_{d|n} (pod(d)/sigma(d))where pod(k) = the
 *          product of the divisors of k (A007955) and sigma(k) = the sum of
 *          the divisors of k (A000203).
 * https://oeis.org/A324984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 35, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   factor_u64, gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A324984: a(n) = numerator of Sum_{d|n} (pod(d)/sigma(d)) */
/* Where pod(k) = product of divisors of k and sigma(k) = sum of divisors of k */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/pow_uint.h"
#include "../utils/factor_u64.h"

static void dfs_divisors(
    int idx,
    int k,
    uint64_t cur_exp[64],
    const uint64_t *primes,
    const uint64_t *exps,
    seq_t tau_cur,
    seq_t *num,
    seq_t *den)
{
    if (idx == k) {
        /* Compute pod and sigma for the current divisor */
        seq_t pod = 1;
        seq_t sigma = 1;
        for (int i = 0; i < k; ++i) {
            uint64_t e = cur_exp[i];
            /* exp_total = e * tau_cur / 2  (safe: tau_cur small) */
            unsigned long long tau_ll = (unsigned long long)tau_cur;
            unsigned long long exp_total = e * tau_ll / 2;
            seq_t part_pod = pow_uint(primes[i], exp_total);
            pod *= part_pod;

            seq_t numerator = pow_uint(primes[i], e + 1) - 1;
            seq_t denom = primes[i] - 1;
            seq_t part_sigma = numerator / denom;
            sigma *= part_sigma;
        }

        /* Add pod/sigma to the cumulative fraction */
        seq_t t_num = pod;
        seq_t t_den = sigma;
        seq_t new_num = (*num) * t_den + t_num * (*den);
        seq_t new_den = (*den) * t_den;
        seq_t g = gcd(new_num, new_den);
        *num = new_num / g;
        *den = new_den / g;
        return;
    }

    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        cur_exp[idx] = e;
        dfs_divisors(
            idx + 1,
            k,
            cur_exp,
            primes,
            exps,
            tau_cur * (e + 1),
            num,
            den);
    }
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;   /* cast to 64‑bit unsigned for factorization */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(N, primes, exps, &cnt);

    seq_t num = 0;
    seq_t den = 1;
    uint64_t cur_exp[64] = {0};

    dfs_divisors(0, (int)cnt, cur_exp, primes, exps, (seq_t)1, &num, &den);

    return num;   /* numerator of the reduced fraction */
}
