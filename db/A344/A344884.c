/*
 * A344884  a(n) = 1 if A011772(n) divides A344875(n), otherwise 0;
 *          characteristic function of A344974.
 * https://oeis.org/A344884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A011772(n) divides A344875(n) else 0; A011772 by
 *            search over m<2n, A344875 multiplicative (computed mod
 *            A011772(n))
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8327 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t N = (uint64_t)n;
    unsigned __int128 two_n = (unsigned __int128)N * 2;
    uint64_t m = 0;
    for (uint64_t k = 1; k < 2 * N || k == 1; ++k) {
        unsigned __int128 pr = (unsigned __int128)k * (k + 1);
        if (pr % two_n == 0) { m = k; break; }
    }
    if (m == 0) m = 2 * N - 1;

    /* A344875(n) mod m */
    unsigned __int128 res = 1 % m;
    uint64_t t = N;
    uint64_t p = 2;
    while (t > 1) {
        if (p * p > t) p = t;
        if (t % p == 0) {
            unsigned e = 0;
            while (t % p == 0) { t /= p; ++e; }
            unsigned __int128 pw = 1;
            unsigned lim = (p == 2) ? e + 1 : e;
            for (unsigned i = 0; i < lim; ++i) pw = (pw * p) % m;
            unsigned __int128 term = (pw + m - 1) % m;
            res = (res * term) % m;
        }
        if (p == 2) p = 3; else p += 2;
    }
    return (seq_t)(res == 0 ? 1 : 0);
}
