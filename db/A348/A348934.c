/*
 * A348934  Numbers k congruent to 1 or 5 mod 6, for which A348930(k^2) > k^2.
 * https://oeis.org/A348934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over k == 1 or 5 (mod 6); a(n) = n-th such k with
 *            A348930(k^2) > k^2, where A348930(m) = sigma(m) with all factors
 *            of 3 removed
 * Range:     u8 n <= 41, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static bool A348934_test(uint64_t k)
{
    unsigned __int128 sig = 1;
    uint64_t m = k;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            unsigned __int128 t = 1, pw = 1;
            for (int i = 0; i < 2 * e; ++i) {
                pw *= (unsigned __int128)p;
                t += pw;
            }
            while (t % 3 == 0) t /= 3;
            sig *= t;
        }
    }
    if (m > 1) {
        unsigned __int128 pp = (unsigned __int128)m;
        unsigned __int128 t = 1 + pp + pp * pp;
        while (t % 3 == 0) t /= 3;
        sig *= t;
    }
    while (sig % 3 == 0) sig /= 3;
    unsigned __int128 sq = (unsigned __int128)k * (unsigned __int128)k;
    return sig > sq;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t k = 5;
    uint64_t step = 2; /* 5 -> 7 -> 11 -> 13 ... */
    for (;;) {
        if (A348934_test(k)) {
            ++count;
            if (count == n) return (seq_t)k;
        }
        k += step;
        step = 6 - step;
    }
}
