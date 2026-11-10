/*
 * A366429  a(n) = maximum degree of vertices in the distance graph of the
 *          partitions of n.
 * https://oeis.org/A366429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*binomial(A002024(n+1)-1,2) + A000267(A002262(n)) - 1
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A366429_isqrt(uint64_t x) {
    if (x < 2) return x;
    uint64_t lo = 0, hi = (x < 4294967295ULL) ? x : 4294967295ULL;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        if (mid * mid <= x) lo = mid; else hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n + 1;
    /* A002024(m): smallest k with k(k+1) >= 2m */
    unsigned __int128 target = (unsigned __int128)m * 2;
    uint64_t lo = 1, hi = 4294967296ULL;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if ((unsigned __int128)mid * (mid + 1) < target) lo = mid + 1; else hi = mid;
    }
    uint64_t k = lo;
    /* A002262(n): n - r(r+1)/2, r = largest with r(r+1)/2 <= n */
    uint64_t rl = 0, rh = 4294967296ULL;
    while (rl < rh) {
        uint64_t mid = rl + (rh - rl + 1) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= (unsigned __int128)n) rl = mid; else rh = mid - 1;
    }
    uint64_t c = (uint64_t)n - (uint64_t)((unsigned __int128)rl * (rl + 1) / 2);
    /* A000267(c) = floor(sqrt(4c+1)) */
    uint64_t s = A366429_isqrt(4 * c + 1);
    seq_t res = (seq_t)(k - 1) * (seq_t)(k - 2);
    if (k < 2) res = 0;
    res = res + (seq_t)s - 1;
    return res;
}
