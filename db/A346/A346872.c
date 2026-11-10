/*
 * A346872  Irregular triangle read by rows in which row n lists the row
 *          2^(n-1) of A237591, n >= 1.
 * https://oeis.org/A346872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k), the k-th entry of row r = 2^(m-1) of A237591,
 *            where T(r,k) = ceil((r+1)/k - (k+1)/2) - ceil((r+1)/(k+1) -
 *            (k+2)/2); m is the row of this triangle, of length
 *            floor((sqrt(8r+1)-1)/2)
 * Range:     u8 n <= 66, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Validated: partial, pass, 83 terms through n = 83 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static __int128 A346872_ceil_div(__int128 num, __int128 den)
{
    if (num >= 0)
        return (num + den - 1) / den;
    return num / den;
}

/* largest k with k(k+1)/2 <= r */
static uint64_t A346872_rowlen(uint64_t r)
{
    uint64_t lo = 0, hi = ((uint64_t)1 << 32);
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        if (mid * (mid + 1) / 2 <= r)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    uint64_t idx = (uint64_t)n;
    uint64_t m = 0;
    uint64_t rem = idx;
    uint64_t r = 0;
    uint64_t len = 0;
    for (;;) {
        ++m;
        r = (uint64_t)1 << (m - 1);
        len = A346872_rowlen(r);
        if (rem <= len)
            break;
        rem -= len;
    }
    __int128 K = (__int128)rem;
    __int128 N = (__int128)r;
    __int128 A = A346872_ceil_div(2 * (N + 1) - K * (K + 1), 2 * K);
    __int128 B = A346872_ceil_div(2 * (N + 1) - (K + 1) * (K + 2), 2 * (K + 1));
    return (seq_t)(uint64_t)(A - B);
}
