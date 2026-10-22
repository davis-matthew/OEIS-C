/*
 * A349164  a(n) = A064989(A003961(n) / gcd(sigma(n), A003961(n))), where
 *          A003961 shifts the prime factorization of n one step towards
 *          larger primes, while A064989 shifts it back towards smaller
 *          primes, and sigma is the sum of divisors function.
 * https://oeis.org/A349164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 9740, u32 n <= 9740, u64 n <= 9740,
 *            u128 n <= 9740, big any n
 * Helpers:   factor_u64, gcd, next_prime, pow_uint, prev_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A064989(A349161(n))
 *   where
 *     A349161(n) = A003961(n) / gcd(σ(n), A003961(n))
 *     A003961(n) shifts the prime factorization of n towards larger primes
 *     A064989 shifts the prime factorization of m towards smaller primes
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"
#include "../utils/sigma_of.h"
#include "../utils/gcd.h"
#include "../utils/next_prime.h"
#include "../utils/prev_prime.h"

/* main sequence function */
seq_t a(idx_t n)
{
    /* factorisation of n (assumes n fits in uint64) */
    uint64_t n_pr[64], n_ex[64];
    size_t n_cnt;
    factor_u64((uint64_t)n, n_pr, n_ex, &n_cnt);

    /* compute A003961(n) = ∏ next_prime(p_i)^{e_i} */
    seq_t a003961 = 1;
    for (size_t i = 0; i < n_cnt; ++i) {
        uint64_t np = next_prime(n_pr[i]);   /* next prime after n_pr[i] */
        seq_t pw = pow_uint(np, (idx_t)n_ex[i]);
        a003961 *= pw;
    }

    /* σ(n) */
    seq_t sigma_n = sigma_of(n);

    /* A349161(n) = a003961 / gcd(a003961, sigma_n) */
    seq_t g = gcd(a003961, sigma_n);
    seq_t quot = a003961 / g;   /* exact division */

    /* factorisation of the quotient (fits in uint64 for n ≤ 10 000) */
    uint64_t q_pr[64], q_ex[64];
    size_t q_cnt;
    factor_u64((uint64_t)quot, q_pr, q_ex, &q_cnt);

    /* compute A064989(quot) = ∏ prev_prime(p_i)^{e_i} (skip p = 2) */
    seq_t res = 1;
    for (size_t i = 0; i < q_cnt; ++i) {
        uint64_t prev = (q_pr[i] > 2) ? prev_prime(q_pr[i]) : 0;
        if (prev == 0) continue;            /* 2 has no previous prime */
        seq_t pw = pow_uint(prev, (idx_t)q_ex[i]);
        res *= pw;
    }

    return res;
}
