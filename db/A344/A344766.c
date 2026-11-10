/*
 * A344766  a(n) = gcd(sigma(n), A011772(n)).
 * https://oeis.org/A344766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(sigma(n), A011772(n)); A011772(n) found by search for
 *            smallest m with 2n | m(m+1)
 * Range:     u8 n <= 255, u16 n <= 65534, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9495 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A344766_sigma(idx_t n)
{
    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p;
    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t term = 1;
            seq_t pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
            }
            result *= term;
        }
    }
    if (m > 1) {
        result *= ((seq_t)m + 1);
    }
    return result;
}

static uint64_t A344766_tri(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t limit = n * 2u;
    uint64_t m;
    for (m = 1; m < limit; ++m) {
        unsigned __int128 prod = (unsigned __int128)m * (m + 1);
        if (prod % limit == 0)
            return m;
    }
    return limit - 1;
}

seq_t a(idx_t n)
{
    seq_t s = A344766_sigma(n);
    seq_t t = (seq_t)A344766_tri((uint64_t)n);
    while (t != 0) {
        seq_t r = s % t;
        s = t;
        t = r;
    }
    return s;
}
