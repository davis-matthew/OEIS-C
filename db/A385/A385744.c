/*
 * A385744  The number of iterations of the infinitary analog of the totient
 *          function A384247 that are required to reach from n to 1.
 * https://oeis.org/A385744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(A384247(n)) + 1 for n >= 2, a(1) = 0, where A384247(n)
 *            = Product_{p^e || n} (p^e - p^(e - 2^v2(e)))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9724 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static uint64_t A385744_pw(uint64_t b, uint64_t e)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1ULL) r *= b;
        e >>= 1;
        if (e) b *= b;
    }
    return r;
}

static uint64_t A385744_inftot(uint64_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    uint64_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t t = e & (~e + 1ULL); /* 2^v2(e) */
        result *= A385744_pw(p, e) - A385744_pw(p, e - t);
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    uint64_t cur = (uint64_t)n;
    seq_t steps = 0;
    while (cur > 1) {
        cur = A385744_inftot(cur);
        ++steps;
    }
    return steps;
}
