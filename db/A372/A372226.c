/*
 * A372226  a(n) = Sum_{k=1..n} sigma_2( n/gcd(k,n) ).
 * https://oeis.org/A372226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} φ(d) * σ₂(d)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1626, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 58 terms through n = 9610 (2026-11-01)
 * Helpers:   factor_u64, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (σ₂ = sum of squares of divisors, φ = Euler's totient) */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/totient.h"

static seq_t sigma_k(idx_t m, uint8_t k)
{
    seq_t res = (seq_t)1;
    idx_t n = m;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            idx_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }

            /* compute pk = p^k */
            seq_t pk = (seq_t)1;
            for (idx_t i = 0; i < k; ++i) pk *= (seq_t)p;

            /* sum_{i=0}^{exp} (p^k)^i = 1 + pk + pk^2 + ...   */
            seq_t term = (seq_t)0;
            seq_t pow   = (seq_t)1;
            for (idx_t i = 0; i <= exp; ++i) {
                term += pow;
                pow  *= pk;
            }
            res *= term;
        }
    }

    if (n > 1) {               /* remaining prime factor with exponent 1 */
        /* k is 1 or 3, exp = 1 */
        seq_t pk = (seq_t)1;
        for (idx_t i = 0; i < k; ++i) pk *= (seq_t)n;

        seq_t term = (seq_t)0;
        seq_t pow   = (seq_t)1;
        for (idx_t i = 0; i <= (idx_t)1; ++i) {
            term += pow;
            pow  *= pk;
        }
        res *= term;
    }

    return res;
}

/* Helper: recursively enumerate all divisors of n using its prime factorisation
 * and accumulate φ(d) * σ₂(d). */
static seq_t _div_sum(const uint64_t *primes, const int *exps,
                      size_t cnt, seq_t cur)
{
    if (cnt == 0) {
        seq_t phi_val   = totient((idx_t)cur);     /* φ(cur)   */
        seq_t sig2_val  = sigma_k((idx_t)cur, 2); /* σ₂(cur)  */
        return phi_val * sig2_val;
    }
    seq_t sum   = 0;
    seq_t val   = cur;
    for (int e = 0; e <= exps[0]; ++e) {
        sum += _div_sum(primes + 1, exps + 1, cnt - 1, val);
        val *= primes[0];
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Factor n into primes and exponents */
    uint64_t prime_fact[64];
    uint64_t exp_fact[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, prime_fact, exp_fact, &cnt);

    /* Convert exponent array to int for recursion */
    int exps[64];
    for (size_t i = 0; i < cnt; ++i) exps[i] = (int)exp_fact[i];

    /* Compute the sum over divisors */
    return _div_sum(prime_fact, exps, cnt, (seq_t)1);
}
