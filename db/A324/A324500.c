/*
 * A324500  a(n) = denominator of Sum_{d|n} sigma(d)/tau(d) where sigma(k) =
 *          the sum of the divisors of k (A000203) and tau(k) = the number of
 *          divisors of k (A000005).
 * https://oeis.org/A324500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of Sum_{d|n} sigma(d)/tau(d)
 * Range:     u8 n <= 18, u16 n <= 2858, u32 n <= 8595, u64 n <= 8595,
 *            u128 n <= 8595, big any n
 * Helpers:   big_pow, factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    /* Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* global product of the sums over prime powers */
    seq_t g_num = 1;
    seq_t g_den = 1;

    for (size_t pi = 0; pi < pcnt; ++pi) {
        uint64_t p   = primes[pi];
        uint64_t k   = exps[pi];
        seq_t   p_seq   = (seq_t)p;
        seq_t   p_minus1 = (seq_t)(p - 1);

        /* Compute S_p = sum_{e=0}^{k} sigma(p^e)/tau(p^e) */
        seq_t sum_num = 0;
        seq_t sum_den = 1;

        for (uint64_t e = 0; e <= k; ++e) {
            seq_t exp_seq = (seq_t)(e + 1);

            /* sigma(p^e) = (p^{e+1} - 1) / (p - 1) */
            seq_t pow_val = big_pow(p_seq, exp_seq);          /* p^{e+1} */
            seq_t sigma   = (pow_val - (seq_t)1) / p_minus1;  /* exact division */

            /* tau(p^e) = e + 1 */
            seq_t tau = (seq_t)(e + 1);

            /* reduce sigma/tau */
            seq_t g = gcd(sigma, tau);
            sigma /= g;
            tau   /= g;

            /* add sigma/tau to current sum */
            seq_t g1 = gcd(sum_den, tau);
            seq_t lcm = sum_den / g1 * tau;                     /* lcm of denominators */

            seq_t new_num = sum_num * (tau / g1) + sigma * (sum_den / g1);
            seq_t new_den = lcm;

            seq_t g2 = gcd(new_num, new_den);
            sum_num = new_num / g2;
            sum_den = new_den / g2;
        }

        /* multiply the result for this prime into the global product */
        seq_t new_num = g_num * sum_num;
        seq_t new_den = g_den * sum_den;

        seq_t g3 = gcd(new_num, new_den);
        g_num = new_num / g3;
        g_den = new_den / g3;
    }

    return g_den;
}
