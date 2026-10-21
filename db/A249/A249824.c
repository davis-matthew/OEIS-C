/*
 * A249824  Permutation of natural numbers: a(n) =
 *          A078898(A003961(A003961(2*n))).
 * https://oeis.org/A249824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { k ≤ A003961(A003961(2*n)) : 5 | k, k not divisible by
 *            2 or 3 }. Equivalently, b = A003961(A003961(2*n)) a(n) = ⌊b/5⌋ −
 *            ⌊b/10⌋ − ⌊b/15⌋ + ⌊b/30⌋
 * Range:     u8 n <= 7, u16 n <= 215, u32 n <= 255, u64 n <= 255,
 *            u128 n <= 255, big any n
 * Helpers:   is_prime_d
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime_d.h"

/* next prime after x */
static uint64_t next_prime(uint64_t x)
{
    uint64_t cand = x + 1;
    while (1) {
        if (is_prime_d(cand)) return cand;
        ++cand;
    }
}

/* power: base^exp, exp ≥ 0 */
static seq_t pow_uint(seq_t base, int exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* compute A003961(A003961(2*n)) directly */
static seq_t compute_b(idx_t n)
{
    unsigned long long m = 2ULL * (unsigned long long)n;   /* 2*n  (fits in 64‑bit) */
    seq_t res = 1;

    /* factorization of m */
    unsigned long long temp = m;

    /* handle factor 2 separately to cap the exponent */
    int e = 0;
    while ((temp & 1ULL) == 0) {          /* temp % 2 == 0 */
        temp >>= 1;
        ++e;
    }
    if (e > 0) {
        uint64_t p2   = next_prime(2);            /* 3 */
        uint64_t p3   = next_prime(p2);           /* 5 */
        seq_t   base = (seq_t)p3;
        res *= pow_uint(base, e);
    }

    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            uint64_t p2 = next_prime(p);          /* next prime after p           */
            uint64_t p3 = next_prime(p2);          /* next prime after that        */
            seq_t   base = (seq_t)p3;
            res *= pow_uint(base, exp);
        }
    }

    /* remaining prime factor > 1 */
    if (temp > 1) {
        uint64_t p2 = next_prime(temp);
        uint64_t p3 = next_prime(p2);
        seq_t   base = (seq_t)p3;
        res *= base;   /* exponent is 1 */
    }

    return res;
}

/* main sequence function */
seq_t a(idx_t n)
{
    seq_t b         = compute_b(n);
    seq_t five      = (seq_t)5, ten = (seq_t)10, fifteen = (seq_t)15, thirty = (seq_t)30;
    seq_t part1 = b / five;
    seq_t part2 = b / ten;
    seq_t part3 = b / fifteen;
    seq_t part4 = b / thirty;
    return part1 - part2 - part3 + part4;
}
