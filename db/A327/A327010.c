/*
 * A327010  Indices of records in A327008.
 * https://oeis.org/A327010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m such that A327008(m) = n-1; found by a search:
 *            bisect on the smooth growth of A327008, then scan a window of
 *            width O(sqrt(m)) around the crossing point for the first exact
 *            hit
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* A327008(m) with 64-bit arithmetic; valid while 2*m^2 < 2^64 */
static uint64_t A327010_f64(uint64_t m)
{
    uint64_t nsq = m * m;
    uint64_t x = nsq;
    uint64_t steps = 0;
    while (x > m) {
        uint64_t two_x = x << 1;
        uint64_t nx = x / 2 + nsq / two_x;
        if ((x & 1) && (nsq % two_x >= x))
            ++nx;
        x = nx;
        ++steps;
    }
    return steps;
}

/* A327008(m) with 128-bit arithmetic */
static uint64_t A327010_f128(uint64_t m)
{
    unsigned __int128 nsq = (unsigned __int128)m * m;
    unsigned __int128 x = nsq;
    uint64_t steps = 0;
    while (x > (unsigned __int128)m) {
        unsigned __int128 two_x = x << 1;
        unsigned __int128 nx = x / 2 + nsq / two_x;
        if ((x & 1) && (nsq % two_x >= x))
            ++nx;
        x = nx;
        ++steps;
    }
    return steps;
}

static uint64_t A327010_f(uint64_t m)
{
    if (m <= 3000000000ULL)
        return A327010_f64(m);
    return A327010_f128(m);
}

static uint64_t A327010_isqrt(uint64_t v)
{
    uint64_t r = 0, bit = (uint64_t)1 << 62;
    while (bit > v) bit >>= 2;
    while (bit) {
        if (v >= r + bit) { v -= r + bit; r = (r >> 1) + bit; }
        else r >>= 1;
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    uint64_t k = (uint64_t)n - 1;
    uint64_t hi = 1;
    while (A327010_f(hi) < k && hi < ((uint64_t)1 << 40))
        hi *= 2;
    uint64_t lo = 1;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (A327010_f(mid) >= k)
            hi = mid;
        else
            lo = mid + 1;
    }
    /* lo is near the threshold; exact first hit lies in a narrow band */
    uint64_t w = 8 * A327010_isqrt(lo) + 64;
    uint64_t start = (lo > w) ? lo - w : 1;
    uint64_t end = lo + w;
    for (uint64_t m = start; m <= end; m++) {
        if (A327010_f(m) == k)
            return (seq_t)m;
    }
    /* fallback: continue upward */
    for (uint64_t m = end + 1;; m++) {
        if (A327010_f(m) == k)
            return (seq_t)m;
    }
}
