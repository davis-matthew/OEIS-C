/*
 * A344969  a(n) = gcd(A011772(n), A344875(n)).
 * https://oeis.org/A344969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A011772(n), A344875(n)); A011772 by search for
 *            smallest m with n | m(m+1)/2, A344875 multiplicative with
 *            f(2^e)=2^(e+1)-1, f(p^e)=p^e-1
 * Range:     u8 n <= 24, u16 n <= 1067, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 109 terms through n = 9488 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A011772_a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t limit = N * 2;
    for (seq_t m = 1; m < limit; ++m) {
        seq_t t = m * (m + 1) / 2;
        if (t % N == 0) return m;
    }
    return limit - 1;
}

static seq_t A344875_a(idx_t n)
{
    seq_t ans = 1;
    uint64_t tmp = (uint64_t)n;
    uint64_t p;
    for (p = 2; p * p <= tmp; p += (p == 2 ? 1 : 2)) {
        if (tmp % p == 0) {
            unsigned e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            seq_t pw = 1;
            for (unsigned i = 0; i < e; ++i) pw *= p;
            if (p == 2) ans *= (pw * 2 - 1);
            else ans *= (pw - 1);
        }
    }
    if (tmp > 1) {
        if (tmp == 2) ans *= 3;
        else ans *= (seq_t)(tmp - 1);
    }
    return ans;
}

static seq_t A344969_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t r = x % y;
        x = y;
        y = r;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    return A344969_gcd(A011772_a(n), A344875_a(n));
}
