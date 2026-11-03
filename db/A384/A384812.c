/*
 * A384812  If n = Product prime(i)^e(i) then a(n) = Sum prime(e(i)).
 * https://oeis.org/A384812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over prime exponents e of prime(e) where n = ∏
 *            prime_i ^ e_i, with a(1) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9340 (2026-11-03)
 * Helpers:   gcd_u64, is_prime_u64, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"

/* Pollard–Rho: find a non‑trivial divisor of n (n composite) */
static uint64_t pollard(uint64_t n) {
    if (n % 2 == 0) return 2;
    uint64_t c = 1;
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mulmod(x, x, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd_u64(diff, n);
        }
        if (d != n) return d;
        ++c;  /* try another constant */
    }
}

/* recursive smallest prime factor */
static uint64_t smallest_prime_factor(uint64_t n) {
    if (n % 2 == 0) return 2;
    if (is_prime_u64(n)) return n;
    uint64_t d = pollard(n);
    uint64_t s1 = smallest_prime_factor(d);
    uint64_t s2 = smallest_prime_factor(n / d);
    return (s1 < s2) ? s1 : s2;
}

/* 64 primes – enough for exponents that appear in uint64_t factorizations. */
static const uint32_t _prime_of_exp[64] = {
    2,   3,   5,   7,   11,  13,  17,  19,  23,  29,
    31,  37,  41,  43,  47,  53,  59,  61,  67,  71,
    73,  79,  83,  89,  97,  101, 103, 107, 109, 113,
    127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
    233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
    283, 293, 307, 311
};

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n <= 1u)
        return (seq_t)0;

    uint64_t cur = n;          /* work with native 64‑bit integers   */
    seq_t     sum = (seq_t)0;   /* running total                    */

    while (cur > 1u) {
        uint64_t p = smallest_prime_factor(cur);  /* smallest prime factor   */
        uint64_t exp = 0;
        while (cur % p == 0u) {
            cur /= p;
            ++exp;
        }
        /* exp >= 1; map e -> prime(e) via pre‑computed table */
        sum += (seq_t)_prime_of_exp[(size_t)(exp - 1u)];
    }
    return sum;
}
