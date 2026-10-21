/*
 * A296076  Least number with the same prime signature as 1 + A002322(n),
 *          where A002322 is Carmichael's lambda.
 * https://oeis.org/A296076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A046523(1 + A002322(n))
 * Range:     u8 n <= 976, u16 n <= 9619, u32 n <= 9619, u64 n <= 9619,
 *            u128 n <= 9619, big any n
 * Helpers:   factor_u64, seq_gcd, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least integer with the same prime signature as 1 + Carmichael lambda(n). */
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* Compute Carmichael lambda(n) */
    seq_t lam = 1;
    uint64_t N = (uint64_t)n;
    uint64_t fac[64];
    uint64_t exp[64];
    size_t cnt = 0;
    factor_u64(N, fac, exp, &cnt);
    for (size_t i = 0; i < cnt; i++) {
        uint64_t p = fac[i];
        uint64_t e = exp[i];
        seq_t part;
        if (p == 2) {
            if (e == 1)
                part = 1;
            else if (e == 2)
                part = 2;
            else
                part = seq_pow(2, e - 2);
        } else {
            seq_t p_pow = seq_pow(p, e - 1);          /* p^(e-1) */
            part = seq_mul(p_pow, (seq_t)(p - 1));    /* * (p-1) */
        }
        seq_t g = seq_gcd(lam, part);
        lam = seq_mul(lam / g, part);                 /* lcm(lam, part) */
    }

    seq_t target = lam + 1;                           /* 1 + lambda(n) */

    /* Factor 1 + lambda(n) and collect exponents */
    uint64_t target64 = (uint64_t)target;             /* fits in 64-bit for n up to 10000 */
    uint64_t primes[64];
    uint64_t exponents[64];
    size_t pcnt = 0;
    factor_u64(target64, primes, exponents, &pcnt);

    uint8_t sig[64];
    for (size_t i = 0; i < pcnt; i++)
        sig[i] = (uint8_t)exponents[i];

    /* Sort exponents in decreasing order */
    for (size_t i = 0; i < pcnt; i++)
        for (size_t j = i + 1; j < pcnt; j++)
            if (sig[i] < sig[j]) {
                uint8_t tmp = sig[i];
                sig[i] = sig[j];
                sig[j] = tmp;
            }

    /* Assign exponents to the smallest primes to minimize the product */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
        19ULL, 23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL,
        47ULL, 53ULL, 59ULL, 61ULL, 67ULL, 71ULL
    };

    seq_t result = 1;
    for (size_t i = 0; i < pcnt; i++)
        result = seq_mul(result, seq_pow(small_primes[i], sig[i]));

    return result;
}
