/*
 * A324350  Square array read by antidiagonals: A(x,y) =
 *          gcd(A276086(x),A276086(y)), for x, y >= 0.
 * https://oeis.org/A324350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(x,y) = gcd(A276086(x),A276086(y)) = prod_i
 *            prime(i+1)^min(d_i(x),d_i(y)), where d_i are primorial-base
 *            digits; (x,y) read by antidiagonals: n = k(k+1)/2 + x, y = k - x
 * Range:     u8 n <= 611, u16 n <= 7079, u32 n <= 7259, u64 n <= 7259,
 *            u128 n <= 7259, big any n
 * Validated: standard, pass, 136 terms through n = 6739 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t A324350_pw(uint32_t p, uint32_t e)
{
    seq_t r = 1;
    while (e--) r *= (seq_t)p;
    return r;
}

seq_t a(idx_t n)
{
    static const uint32_t pr[22] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79};
    uint64_t nn = (uint64_t)n;
    uint64_t lo = 0, hi = 4294967295ULL;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        if (mid * (mid + 1) / 2 <= nn) lo = mid; else hi = mid - 1;
    }
    uint64_t k = lo;
    uint64_t x = nn - k * (k + 1) / 2;
    uint64_t y = k - x;

    seq_t result = 1;
    for (size_t i = 0; i < 22 && x > 0 && y > 0; ++i) {
        uint32_t dx = (uint32_t)(x % pr[i]);
        uint32_t dy = (uint32_t)(y % pr[i]);
        x /= pr[i];
        y /= pr[i];
        uint32_t m = dx < dy ? dx : dy;
        if (m) result *= A324350_pw(pr[i], m);
    }
    return result;
}
