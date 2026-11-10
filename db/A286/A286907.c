/*
 * A286907  {0->00,1->01}-transform of the Sturmian word A080764.
 * https://oeis.org/A286907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2m) = A080764(m-1), a(2m-1) = 0, where A080764(k) =
 *            floor((k+2)/sqrt(2)) - floor((k+1)/sqrt(2))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9718 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A286907_isqrt128(unsigned __int128 x)
{
    uint64_t lo = 0, hi = ((uint64_t)1 << 63), res = 0;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            res = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return res;
}

static uint64_t A286907_fl(uint64_t m)
{
    unsigned __int128 s = (unsigned __int128)m * m;
    s /= 2;
    return A286907_isqrt128(s);
}

seq_t a(idx_t n)
{
    if (n == 0 || (n & 1)) return (seq_t)0;
    uint64_t k = (uint64_t)(n / 2) - 1;
    uint64_t v = A286907_fl(k + 2) - A286907_fl(k + 1);
    return (seq_t)v;
}
