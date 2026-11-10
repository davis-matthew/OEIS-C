/*
 * A380379  Least k for which A380377(k) = n, i.e., the least number of voters
 *          for which n supporters are needed to win an election with rules as
 *          in A380377.
 * https://oeis.org/A380379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k with A380377(k) = n; found by a search upward
 *            from about 4n - 12*sqrt(n), using A380377(k) = min_{1<=d<=k}
 *            (cost of winning floor(d/2)+1 balanced districts)
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9581 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A380379_f(idx_t n)
{
    idx_t best = n + 1;
    for (idx_t d = 1; d <= n; ++d) {
        idx_t q = n / d;
        idx_t r = n - q * d;
        idx_t small_cnt = d - r;
        idx_t t_small = (q / 2) + 1;
        idx_t t_large = ((q + 1) / 2) + 1;
        idx_t win_needed = (d / 2) + 1;
        idx_t nv;
        if (small_cnt >= win_needed) nv = win_needed * t_small;
        else nv = small_cnt * t_small + (win_needed - small_cnt) * t_large;
        if (nv < best) best = nv;
    }
    return best;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t m = 4 * n;
    idx_t s = 0;
    while ((s + 1) * (s + 1) <= m) ++s;
    idx_t sub = 6 * s + 10;
    idx_t k = (m > sub) ? (m - sub) : 1;
    if (k < 1) k = 1;
    for (;; ++k) {
        if (A380379_f(k) == n) return (seq_t)k;
    }
}
