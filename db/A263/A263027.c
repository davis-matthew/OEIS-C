/*
 * A263027  a(n) = A002322(n) + 1, where A002322 is Carmichael lambda.
 * https://oeis.org/A263027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Carmichael lambda(n) + 1
 * Range:     u8 n <= 256, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = λ(n) + 1.  λ(n) is the Carmichael function. */
#include <stdint.h>
#include "../utils/factor_u64.h"

static inline uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static inline uint64_t pow_uint64(uint64_t base, unsigned exp)
{
    uint64_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* n is 1-based; the function is called only for n >= 1. */
    uint64_t nn = (uint64_t)n;
    if (nn == 0) return (seq_t)0;          /* safety fall-back */

    /* Special case: λ(1) = 1. */
    if (nn == 1) return (seq_t)2;          /* 1 + 1 */

    /* Factor nn */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    uint64_t lam = 1; /* λ(n) will be accumulated here */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t part;

        if (p == 2) {
            if (e == 1) {
                part = 1;                /* λ(2) = 1 */
            } else if (e == 2) {
                part = 2;                /* λ(4) = 2 */
            } else {
                /* λ(2^e) = 2^(e-2) for e >= 3 */
                part = pow_uint64(2ULL, (unsigned)(e - 2));
            }
        } else {
            /* λ(p^e) = φ(p^e) = p^(e-1) * (p-1)  for odd p */
            uint64_t p_pow = pow_uint64(p, (unsigned)(e - 1));
            part = p_pow * (p - 1ULL);
        }

        /* lcm(acc, part) = acc/gcd * part */
        uint64_t g = gcd64(lam, part);
        /* use 128-bit intermediate to avoid overflow */
        unsigned __int128 t = (unsigned __int128)(lam / g) * part;
        lam = (uint64_t)t;
    }

    /* λ(n) + 1 */
    return (seq_t)(lam + 1ULL);
}
