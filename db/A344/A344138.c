/*
 * A344138  a(n) = Sum_{x_1|n, x_2|n, x_3|n, x_4|n} gcd(x_1,x_2,x_3,x_4).
 * https://oeis.org/A344138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} phi(n/d) * tau(d)^4
 * Range:     u8 n <= 5, u16 n <= 119, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9674 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void rec_func(int idx, idx_t d_acc, uint64_t tau_acc, seq_t phi_acc, seq_t *ans,
                     const uint64_t *primes, const int *exps, int num)
{
    if (idx == num) {
        seq_t t4 = (seq_t)(tau_acc * tau_acc * tau_acc * tau_acc);
        *ans += phi_acc * t4;
        return;
    }
    uint64_t p = primes[idx];
    int e_max = exps[idx];
    idx_t pow_p = 1;
    for (int e = 0; e <= e_max; ++e) {
        int e_diff = e_max - e;
        seq_t phi_factor = 1;
        if (e_diff > 0) {
            seq_t p_pow_exp = 1;
            for (int k = 0; k < e_diff - 1; ++k) {
                p_pow_exp *= (seq_t)p;
            }
            phi_factor = (seq_t)(p - 1) * p_pow_exp;
        }
        seq_t next_phi = phi_acc * phi_factor;
        uint64_t next_tau = tau_acc * (uint64_t)(e + 1);
        rec_func(idx + 1, (idx_t)(d_acc * pow_p), next_tau, next_phi, ans,
                 primes, exps, num);
        pow_p *= p;
    }
}

static void rec_func(int idx, idx_t d_acc, uint64_t tau_acc, seq_t phi_acc, seq_t *ans,
                     const uint64_t *primes, const int *exps, int num);

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    uint64_t temp = (uint64_t)n;
    uint64_t primes[16];
    int exps[16];
    int num = 0;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            primes[num] = p;
            exps[num] = cnt;
            ++num;
        }
    }
    if (temp > 1) {
        primes[num] = temp;
        exps[num] = 1;
        ++num;
    }

    seq_t ans = 0;
    rec_func(0, (idx_t)1, 1, (seq_t)1, &ans,
             primes, exps, num);
    return ans;
}
