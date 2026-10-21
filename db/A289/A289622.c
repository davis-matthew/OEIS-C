/*
 * A289622  Compound filter (prime signature & Carmichael's lambda): a(n) =
 *          P(A046523(n), A002322(n)), where P(n,k) is sequence A000027 used
 *          as a pairing function.
 * https://oeis.org/A289622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*(2 + ((A046523(n)+A002322(n))^2) - A046523(n) -
 *            3*A002322(n)) with A046523(n) = Π_{i>=1} prime_i^{e_i}, where
 *            the exponents e_i are the exponents from the prime factorisation
 *            of n, sorted in decreasing order. A002322(n) is Carmichael's
 *            λ(n)
 * Range:     u8 n <= 15, u16 n <= 358, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <math.h>

/* utility functions from the build environment */
#include "../utils/factor_u64.h"
#include "../utils/gcd_u64.h"

/* small gcd for uint64_t */

/* first few primes (enough for all exponents needed up to n≈10⁹) */
static const uint64_t FIRST_PRIMES[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
    23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL,
    59ULL, 61ULL, 67ULL, 71ULL, 73ULL, 79ULL, 83ULL, 89ULL,
    97ULL, 101ULL
};
static const size_t NUM_FIRST_PRIMES =
    sizeof(FIRST_PRIMES) / sizeof(FIRST_PRIMES[0]);

/* compute Carmichael's λ(n) using the prime factorization given by factor_u64 */
static uint64_t carmichael_lambda(uint64_t n)
{
    if (n <= 1) return 1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    uint64_t lambda = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t lam_i;

        if (p == 2) {
            if (e == 1) lam_i = 1;
            else if (e == 2) lam_i = 2;
            else lam_i = 1ULL << (e - 2);           /* 2^(e-2) for e≥3 */
        } else {
            /* λ(p^e) = φ(p^e) = p^(e-1)*(p-1) */
            uint64_t pe_minus1 = 1;
            for (uint64_t k = 0; k < e - 1; ++k) pe_minus1 *= p;
            lam_i = pe_minus1 * (p - 1);
        }
        /* lcm(lambda, lam_i) = lambda / gcd(lambda,lam_i) * lam_i */
        uint64_t g = gcd_u64(lambda, lam_i);
        lambda = (lambda / g) * lam_i;
    }
    return lambda;
}

/* main function implementing the OEIS sequence */
seq_t a(idx_t n)
{
    if (n <= 1) return 1;   /* trivial base case */

    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* sort exponents descending (simple selection sort) */
    for (size_t i = 0; i < cnt; ++i) {
        size_t max_idx = i;
        for (size_t j = i + 1; j < cnt; ++j) {
            if (exps[j] > exps[max_idx]) max_idx = j;
        }
        if (max_idx != i) {
            uint64_t tmp = exps[i];
            exps[i] = exps[max_idx];
            exps[max_idx] = tmp;
        }
    }

    /* compute A046523(n) */
    seq_t a046523_val = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t part = 1;
        for (uint64_t k = 0; k < exps[i]; ++k) {
            part *= (seq_t)FIRST_PRIMES[i];
        }
        a046523_val *= part;
    }

    /* compute Carmichael's λ(n) */
    uint64_t lambda = carmichael_lambda((uint64_t)n);
    seq_t lam = (seq_t)lambda;

    /* apply the given formula */
    seq_t sum = a046523_val + lam;
    seq_t sum_sq = sum * sum;
    seq_t result = (seq_t)2 + sum_sq - a046523_val - (seq_t)3 * lam;
    result /= 2;
    return result;
}
