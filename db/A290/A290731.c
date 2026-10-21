/*
 * A290731  Number of distinct values of X*(X+1) mod n.
 * https://oeis.org/A290731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = number of distinct values of X*(X+1) mod n
 *            Multiplicative: a(2^e) = 2^(e-1) a(p^e) = 1 + floor(p^(e+1) /
 *            (2p+2)) for odd prime p
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* compute 128‑bit power: base^exp, exp is < 128 */
static inline unsigned __int128 pow_u128(unsigned __int128 base, idx_t exp)
{
    unsigned __int128 res = 1;
    while (exp) {
        if (exp & 1U) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        idx_t e   = exps[i];
        seq_t f;

        if (p == 2) {
            /* 2^(e-1) */
            f = ((seq_t)1) << (e - 1);
        } else {
            /* 1 + floor(p^(e+1) / (2(p+1))) */
            unsigned __int128 num = pow_u128(p, e + 1);
            unsigned __int128 denom = (unsigned __int128)2 * (p + 1);
            seq_t quot = (seq_t)(num / denom);
            f = quot + 1;
        }

        result *= f;
    }

    return result;
}
