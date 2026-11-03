/*
 * A304339  Fixed point of f starting with n, where f(x) = x/(largest perfect
 *          power divisor of x).
 * https://oeis.org/A304339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 88 terms through n = 877 (2026-11-01)
 * Helpers:   ipow_u64_u32_to_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A304339: fixed point of f(n) where
   f(x) = x / (largest perfect power divisor of x). The function
   repeatedly applies f until a fixed point (no perfect power
   divisor > 1) is reached. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_u64_u32_to_u64.h"

static seq_t max_perfect_power_divisor(uint64_t val)
{
    if (val <= 1) return (seq_t)1;

    /* factorization: store up to 64 prime factors */
    uint64_t primes[64];
    unsigned exps[64];
    unsigned pcnt = 0;
    unsigned max_exp = 0;

    /* factor 2 */
    if ((val & 1ULL) == 0) {
        unsigned e = 0;
        while ((val & 1ULL) == 0) {
            val >>= 1;
            ++e;
        }
        primes[pcnt] = 2;
        exps[pcnt] = e;
        if (e > max_exp) max_exp = e;
        ++pcnt;
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= val; p += 2) {
        if (val % p == 0) {
            unsigned e = 0;
            while (val % p == 0) {
                val /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt] = e;
            if (e > max_exp) max_exp = e;
            ++pcnt;
        }
    }

    if (val > 1) {
        primes[pcnt] = val;
        exps[pcnt] = 1;
        if (1 > max_exp) max_exp = 1;
        ++pcnt;
    }

    uint64_t best = 1;
    for (unsigned k = 2; k <= max_exp; ++k) {
        uint64_t d = 1;
        for (unsigned i = 0; i < pcnt; ++i) {
            unsigned t = (exps[i] / k) * k;
            if (t == 0) continue;
            d *= ipow_u64_u32_to_u64(primes[i], t);
        }
        if (d > best) best = d;
    }
    return (seq_t)best;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety, though n >= 1 by specification */

    seq_t cur = (seq_t)n;
    while (true) {
        uint64_t cur64 = (uint64_t)cur;          /* cur fits in 64 bits for this sequence */
        seq_t d = max_perfect_power_divisor(cur64);
        if (d == (seq_t)1) return cur;          /* fixed point reached */
        cur /= d;
    }
}
