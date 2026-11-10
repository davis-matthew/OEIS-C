/*
 * A344976  a(n) = A344878(n) - A011772(n).
 * https://oeis.org/A344976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A344878(n) - A011772(n), where A344878(n) = lcm over p^e
 *            || n of p^(e+[p==2]) - 1, and A011772(n) = least m>=1 with n |
 *            m(m+1)/2 (found by search over m < 2n)
 * Range:     i8 n <= 91, i16 n <= 16384, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 108 terms through n = 9147 (2026-11-10)
 * Helpers:   factor_u64, gcd, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/ipow.h"

static seq_t A344976_lcm(seq_t x, seq_t y)
{
    seq_t g = gcd(x, y);
    return (x / g) * y;
}

static seq_t A344976_A344878(idx_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);
    seq_t ans = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t ex = (seq_t)(e + (p == 2 ? 1 : 0));
        seq_t val = ipow((seq_t)p, ex) - (seq_t)1;
        ans = A344976_lcm(ans, val);
    }
    return ans;
}

static idx_t A344976_A011772(idx_t n)
{
    unsigned __int128 mod = (unsigned __int128)n * 2u;
    idx_t limit = n * 2u;
    for (idx_t m = 1; m < limit; ++m) {
        unsigned __int128 prod = (unsigned __int128)m * (unsigned __int128)(m + 1);
        if (prod % mod == 0)
            return m;
    }
    return limit - 1;
}

seq_t a(idx_t n)
{
    seq_t x = A344976_A344878(n);
    idx_t m = A344976_A011772(n);
    return x - (seq_t)m;
}
