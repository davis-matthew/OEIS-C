/*
 * A286571  Compound filter (prime signature of n & n/gcd(n, sigma(n))): a(n)
 *          = P(A046523(n), A017666(n)), where P(n,k) is sequence A000027 used
 *          as a pairing function.
 * https://oeis.org/A286571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*(2 + ((A046523(n)+A017666(n))^2) - A046523(n) -
 *            3*A017666(n)) where A046523(n) is the prime signature mapped to
 *            the first primes (exponents sorted in non‑increasing order) and
 *            A017666(n) = n / gcd(n, sigma(n))
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, get_prime, seq_gcd, seq_mul, seq_pow, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include "../utils/get_prime.h"
#include "../utils/sigma_of.h"
#include "../utils/seq_gcd.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Compute A046523(n): prime signature mapped to first primes.
       Extract prime exponents, sort them descending, then multiply
       the first primes raised to those exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Sort exponents in non‑increasing order. */
    for (size_t i = 0; i < cnt; ++i) {
        for (size_t j = i + 1; j < cnt; ++j) {
            if (exps[j] > exps[i]) {
                uint64_t t = exps[i];
                exps[i] = exps[j];
                exps[j] = t;
            }
        }
    }

    seq_t m = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)get_prime((idx_t)(i + 1));          /* i‑th first prime (2,3,5,…) */
        seq_t term = seq_pow(p, (idx_t)exps[i]);            /* p^e */
        m = seq_mul(m, term);                              /* accumulate product */
    }

    /* Compute A017666(n): n / gcd(n, sigma(n)). */
    seq_t sn = (seq_t)n;
    seq_t sigma = sigma_of(n);
    seq_t g = seq_gcd(sn, sigma);
    seq_t p = sn / g;

    /* Apply the closed formula. */
    seq_t sum = m + p;
    seq_t sum_sq = sum * sum;
    seq_t t = sum_sq - m - seq_mul(p, (seq_t)3) + (seq_t)2;
    seq_t ans = t / (seq_t)2;

    return ans;
}
