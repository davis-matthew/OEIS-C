/*
 * A286101  Square array A(n,k) read by antidiagonals: A(n,k) = T(gcd(n,k),
 *          lcm(n,k)), where T(n,k) is sequence A000027 considered as a
 *          two-dimensional table.
 * https://oeis.org/A286101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = T(g, l) with g=gcd(n,k), l=lcm(n,k), T(r,c) =
 *            (r+c-1)*(r+c-2)/2 + r; array read by antidiagonals
 * Range:     u8 n <= 47, u16 n <= 718, u32 n <= 10585, u64 n <= 10585,
 *            u128 n <= 10585, big any n
 * Validated: standard, pass, 107 terms through n = 9153 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A286101_gcd(idx_t x, idx_t y)
{
    while (y) { idx_t t = x % y; x = y; y = t; }
    return x;
}

seq_t a(idx_t n)
{
    /* find diagonal d: d(d-1)/2 < n <= d(d+1)/2 */
    idx_t lo = 1, hi = (idx_t)1 << 32;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        idx_t tri = (mid % 2 == 0) ? (mid / 2) * (mid + 1) : mid * ((mid + 1) / 2);
        if (tri >= n) hi = mid; else lo = mid + 1;
    }
    idx_t d = lo;
    idx_t before = (d % 2 == 0) ? (d / 2) * (d - 1) : ((d - 1) / 2) * d;
    idx_t i = n - before;
    idx_t j = d + 1 - i;
    idx_t g = A286101_gcd(i, j);
    seq_t l = (seq_t)(i / g) * (seq_t)j;
    seq_t D = (seq_t)g + l - 1;      /* diagonal of T(g,l) */
    return D * (D - 1) / 2 + (seq_t)g;
}
