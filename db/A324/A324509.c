/*
 * A324509  a(n) = numerator of Product_{d|n} (sigma(d)/tau(d)) where sigma(k)
 *          = the sum of the divisors of k (A000203) and tau(k) = the number
 *          of divisors of k (A000005).
 * https://oeis.org/A324509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of ∏_{d|n} (sigma(d)/tau(d)) where * sigma(k) =
 *            sum of divisors of k and tau(k) = number of divisors of k
 * Range:     u8 n <= 15, u16 n <= 35, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static void process_divisors(int pos,
                             seq_t curr_sigma, seq_t curr_tau,
                             const uint64_t *prime, const int *exp,
                             int cnt,
                             seq_t *num, seq_t *den)
{
    if (pos == cnt) {
        /* Multiply the current divisor's contribution into the running
           fraction num/den, canceling common factors at each step.
        */
        seq_t sigma = curr_sigma;
        seq_t tau   = curr_tau;

        seq_t g1 = gcd(sigma, *den);
        sigma    /= g1;
        *den     /= g1;

        seq_t g2 = gcd(tau, *num);
        tau      /= g2;
        *num     /= g2;

        *num = (*num) * sigma;
        *den = (*den) * tau;

        seq_t g3 = gcd(*num, *den);
        *num /= g3;
        *den /= g3;
        return;
    }

    uint64_t p = prime[pos];
    for (int e = 0; e <= exp[pos]; ++e) {
        /* Compute sigma part = sum_{k=0}^e p^k */
        seq_t sigma_part = 1;   /* term for k=0 */
        seq_t term = 1;
        for (int k = 1; k <= e; ++k) {
            term *= (seq_t)p;
            sigma_part += term;
        }
        seq_t new_sigma = curr_sigma * sigma_part;
        seq_t new_tau   = curr_tau * (seq_t)(e + 1);
        process_divisors(pos + 1, new_sigma, new_tau,
                         prime, exp, cnt, num, den);
    }
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; for n == 0 we return 0. */
    if (n == 0) {
        return 0;
    }

    /* Run the divisor enumeration and accumulate the reduced fraction. */
    seq_t num = 1;
    seq_t den = 1;

    /* Factor n (small enough that trial division is fast). */
    uint64_t tmp = (uint64_t)n;
    uint64_t prime[20];
    int exp[20];
    int cnt = 0;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            prime[cnt] = p;
            exp[cnt]   = e;
            ++cnt;
        }
    }
    if (tmp > 1) {
        prime[cnt] = tmp;
        exp[cnt]   = 1;
        ++cnt;
    }

    process_divisors(0, 1, 1, prime, exp, cnt, &num, &den);

    /* The fraction has been fully reduced, den should be 1. */
    return num;
}
