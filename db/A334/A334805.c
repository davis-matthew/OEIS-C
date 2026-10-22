/*
 * A334805  a(n) = Product_{d|n} lcm(d, sigma(d)) where sigma(k) is the sum of
 *          divisors of k (A000203).
 * https://oeis.org/A334805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n} lcm(d, sigma(d))
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Recursive helper that enumerates all divisors of n.  It receives
   the prime factorisation of n and the pre‑computed n-th
   sigma contributions for each prime.  The product of all
   lcm(d, sigma(d)) values is accumulated in *product. */
static void rec_divs(int idx, seq_t curr_val, seq_t curr_sigma,
                     seq_t *product,
                     int pf_cnt,
                     const idx_t *primes,
                     const int *exps,
                     seq_t sigma_contrib[][65])
{
    if (idx == pf_cnt) {
        /* base case – one complete divisor (curr_val) and its σ. */
        seq_t g      = gcd(curr_val, curr_sigma);
        seq_t lcm    = (curr_val / g) * curr_sigma;
        *product *= lcm;
        return;
    }

    idx_t p = primes[idx];
    int   max_e = exps[idx];

    /* power = p^k for the current exponent k */
    seq_t power = (seq_t)1;
    for (int k = 0; k <= max_e; ++k) {
        seq_t new_val = curr_val * power;
        seq_t new_sigma = curr_sigma * sigma_contrib[idx][k];
        rec_divs(idx + 1, new_val, new_sigma, product,
                 pf_cnt, primes, exps, sigma_contrib);
        power *= (seq_t)p;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* not used – offset starts at 1 */

    /* --- factor n ----------------------------------------------------- */
    idx_t temp = n;
    idx_t prime_factors[64];
    int   exps[64];
    int   pf_cnt = 0;

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            prime_factors[pf_cnt] = p;
            exps[pf_cnt] = e;
            ++pf_cnt;
        }
    }
    if (temp > 1) {                /* remaining prime factor */
        prime_factors[pf_cnt] = temp;
        exps[pf_cnt] = 1;
        ++pf_cnt;
    }

    /* --- pre‑compute σ contributions for each prime exponent -----------
        sigma_contrib[i][k] = sum_{t=0}^{k} p_i^t  (k <= exponent of prime i) */
    seq_t sigma_contrib[64][65];   /* 65 >= maximum possible exponent + 1 */
    for (int i = 0; i < pf_cnt; ++i) {
        seq_t pow = (seq_t)1;
        seq_t sum = (seq_t)1;
        sigma_contrib[i][0] = sum;
        for (int k = 1; k <= exps[i]; ++k) {
            pow  *= (seq_t)prime_factors[i];
            sum  += pow;
            sigma_contrib[i][k] = sum;
        }
    }

    /* --- enumerate all divisors and accumulate the product -------------
        We start with divisor 1 (value and σ both 1). */
    seq_t product = (seq_t)1;
    rec_divs(0, (seq_t)1, (seq_t)1, &product,
             pf_cnt, prime_factors, exps, sigma_contrib);

    return product;
}
