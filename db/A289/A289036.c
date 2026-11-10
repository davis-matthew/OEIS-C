/*
 * A289036  Positions of 0 in A289035; complement of A289037.
 * https://oeis.org/A289036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least m such that m - #{k : A289037(k) <= m} = n, where
 *            A289037(k) = floor(k*(2+sqrt(2))) - [ {k*sqrt(2)} < sqrt(2)-1 ]
 *            (A289036 is the complement of A289037; found by nested binary
 *            searches)
 * Range:     u8 n <= 21, u16 n <= 748, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9779 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* largest r with r*r <= x, for x = 2*k*k */
static seq_t A289036_fsqrt(seq_t x, seq_t k)
{
    seq_t lo = 0, hi = 2 * k + 1;
    while (lo < hi) {
        seq_t mid = (lo + hi + 1) / 2;
        if (mid * mid <= x) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

/* A289037(k), k >= 1 */
static seq_t A289036_f(seq_t k)
{
    seq_t r = A289036_fsqrt(2 * k * k, k);
    seq_t m = 2 * k + r;
    seq_t k1 = k - 1;
    seq_t lhs = 2 * k1 * k1;
    seq_t rm = r - 1;
    seq_t rhs = rm * rm;
    if (lhs < rhs) m = m - 1;
    return m;
}

/* number of terms of A289037 that are <= m */
static seq_t A289036_ones(seq_t m)
{
    seq_t lo = 0, hi = m / 3 + 2;
    while (lo < hi) {
        seq_t mid = (lo + hi + 1) / 2;
        if (A289036_f(mid) <= m) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t lo = sn, hi = 2 * sn + 2;
    while (lo < hi) {
        seq_t mid = (lo + hi) / 2;
        if (mid - A289036_ones(mid) >= sn) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}
