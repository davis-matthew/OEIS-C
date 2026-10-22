/*
 * A324363  a(n) = numerator of Sum_{d|n} sigma(d)/pod(d) where sigma(k) = the
 *          sum of the divisors of k (A000203) and pod(k) = the product of the
 *          divisors of k (A007955).
 * https://oeis.org/A324363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Σ_{d|n} σ(d)/pod(d) where σ is the
 *            sum‑of‑divisors function and pod(d) is the product of the
 *            divisors of d
 * Range:     u8 n <= 9, u16 n <= 23, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   factor_u64, gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/pow_uint.h"

/* Recursive helper that visits all divisors of n based on its
   prime factorisation.  cur_exp[i] holds the exponent of the
   i‑th prime in the current divisor. */
static void divide_and_sum(size_t idx, size_t pf_cnt,
                           const uint64_t *primes, const uint64_t *exps,
                           int *cur_exp,
                           seq_t *sum_num, seq_t *sum_den)
{
    if (idx == pf_cnt) {
        /* τ(d) = ∏ (e_i+1) */
        seq_t tau = 1;
        for (size_t i = 0; i < pf_cnt; ++i)
            tau *= (seq_t)(cur_exp[i] + 1);

        /* σ(d) = ∏ (p_i^{e_i+1} – 1)/(p_i – 1) */
        seq_t sigma = 1;
        for (size_t i = 0; i < pf_cnt; ++i) {
            int e = cur_exp[i];
            if (e == 0) continue;
            seq_t p_pow = pow_uint(primes[i], (idx_t)(e + 1));
            seq_t term = (p_pow - 1) / (primes[i] - 1);
            sigma *= term;
        }

        /* pod(d) = d^{τ(d)/2} = ∏ p_i^{e_i * τ(d)/2} */
        seq_t pod = 1;
        for (size_t i = 0; i < pf_cnt; ++i) {
            int e = cur_exp[i];
            if (e == 0) continue;
            seq_t exp = (seq_t)e * tau / 2;          /* integer division */
            pod *= pow_uint(primes[i], (idx_t)exp);
        }

        /* Add σ/pod to the running fraction. */
        seq_t g = gcd(*sum_den, pod);
        seq_t term_num = sigma;
        seq_t term_den = pod;

        seq_t a1 = *sum_num * (term_den / g);
        seq_t a2 = term_num * (*sum_den / g);
        seq_t new_num = a1 + a2;
        seq_t new_den = (*sum_den / g) * term_den;

        seq_t g2 = gcd(new_num, new_den);
        *sum_num = new_num / g2;
        *sum_den = new_den / g2;
    } else {
        for (int e = 0; e <= (int)exps[idx]; ++e) {
            cur_exp[idx] = e;
            divide_and_sum(idx + 1, pf_cnt, primes, exps,
                           cur_exp, sum_num, sum_den);
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* prime factorisation of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pf_cnt;
    factor_u64((uint64_t)n, primes, exps, &pf_cnt);

    int cur_exp[64] = {0};
    seq_t sum_num = 0;
    seq_t sum_den = 1;

    divide_and_sum(0, pf_cnt, primes, exps, cur_exp,
                   &sum_num, &sum_den);

    return sum_num;   /* numerator of the reduced sum */
}
