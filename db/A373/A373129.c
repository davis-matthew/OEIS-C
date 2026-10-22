/*
 * A373129  a(n) = Sum_{1 <= x_1, x_2 <= n} sigma( n/gcd(x_1, x_2, n) ).
 * https://oeis.org/A373129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} J_2(d) * sigma(d)
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 1259, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_int64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where J_2(d) = d^2 ∏_{p|d} (1-1/p^2) and sigma(d) is the sum of divisors of d. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow_int64.h"

/* recursive enumeration of all divisors of n */
static void dfs_divisors(const idx_t *primes, const unsigned *exps,
                         size_t faccnt, size_t idx,
                         seq_t cur_j2, seq_t cur_sigma, seq_t *sum) {
    if (idx == faccnt) {
        *sum += cur_j2 * cur_sigma;
        return;
    }
    idx_t p = primes[idx];
    unsigned emax = exps[idx];
    seq_t pow_p = 1;          /* p^k */
    seq_t j2_k = 1, sigma_k = 1; /* multipliers for current prime factor */
    for (unsigned k = 0; k <= emax; ++k) {
        if (k == 0) {
            j2_k = 1;
            sigma_k = 1;
        } else {
            pow_p *= (seq_t)p;   /* now pow_p = p^k */
            seq_t pow_p_k1 = pow_p * (seq_t)p;           /* p^(k+1) */
            seq_t pow_p_2k_2 = pow_int64((seq_t)p, 2 * k - 2); /* p^(2k-2) */
            j2_k = pow_p_2k_2 * ((seq_t)p * (seq_t)p - 1);
            sigma_k = (pow_p_k1 - 1) / ((seq_t)p - 1);
        }
        dfs_divisors(primes, exps, faccnt, idx + 1,
                     cur_j2 * j2_k, cur_sigma * sigma_k, sum);
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* out of OEIS range, but defined as 0 */
    /* factor n */
    const size_t MAX_FACTS = 16;
    idx_t primes[MAX_FACTS];
    unsigned exps[MAX_FACTS];
    size_t faccnt = 0;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            unsigned e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[faccnt] = p;
            exps[faccnt]   = e;
            ++faccnt;
        }
    }
    if (temp > 1) {          /* remaining prime factor */
        primes[faccnt] = temp;
        exps[faccnt]   = 1;
        ++faccnt;
    }

    seq_t sum = 0;
    dfs_divisors(primes, exps, faccnt, 0, 1, 1, &sum);
    return sum;
}
