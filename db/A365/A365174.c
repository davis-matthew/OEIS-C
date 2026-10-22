/*
 * A365174  The sum of divisors d of n such that gcd(d, n/d) is an
 *          exponentially odd number (A268335).
 * https://oeis.org/A365174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   The sequence is multiplicative. For a prime power n = p^e: if e
 *            is even: a(p^e) = 1 + p^e + (p^(e+1) - p)/(p^2 - 1) if e is odd:
 *            a(p^e) = 1 + p^e + (1 + p^(2⋅⌊e/4⌋+1)) · (p^(2⋅⌊(e+1)/4⌋+1) -
 *            p)/(p^2 - 1) For a general n, a(n) = Π a(p_i^e_i) over its prime
 *            power factors
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

static seq_t pow_uint64(uint64_t base, size_t exp)
{
    uint64_t r = 1;
    while (exp) {
        r *= base;
        exp--;
    }
    return (seq_t)r;
}

static seq_t a_prime_power(uint64_t p, idx_t e)
{
    seq_t p_pow_e = pow_uint64(p, (size_t)e);
    seq_t p_pow_e_plus = pow_uint64(p, (size_t)(e + 1));
    if ((e & 1u) == 0) {              /* e even */
        uint64_t p2   = (uint64_t)p * p;
        uint64_t den  = p2 - 1;
        uint64_t numer = (uint64_t)p_pow_e_plus - p;
        seq_t sum = (seq_t)(numer / den);
        return (seq_t)1 + p_pow_e + sum;
    } else {                           /* e odd */
        idx_t f1 = e / 4;              /* ⌊e/4⌋   */
        idx_t f2 = (e + 1) / 4;        /* ⌊(e+1)/4⌋ */
        seq_t k1 = (seq_t)(2 * f1 + 1); /* 2⌊e/4⌋+1 */
        seq_t k2 = (seq_t)(2 * f2 + 1); /* 2⌊(e+1)/4⌋+1 */
        seq_t pow_k1 = pow_uint64(p, (size_t)k1);
        seq_t pow_k2 = pow_uint64(p, (size_t)k2);
        seq_t part1 = (seq_t)1 + pow_k1;
        seq_t part2 = pow_k2 - (seq_t)p;
        uint64_t p2   = (uint64_t)p * p;
        uint64_t den  = p2 - 1;
        uint64_t numer = (uint64_t)part1 * (uint64_t)part2;
        seq_t sum = (seq_t)(numer / den);
        return (seq_t)1 + p_pow_e + sum;
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    seq_t product = (seq_t)1;
    for (size_t i = 0; i < pcnt; ++i) {
        seq_t part = a_prime_power(primes[i], (idx_t)exps[i]);
        product = product * part;
    }
    return product;
}
