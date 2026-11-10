/*
 * A286987  {111->1}-transform of the Sturmian word A080764.
 * https://oeis.org/A286987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=1 if n-1 = floor(m*(10+sqrt(2))/7) for some m>=0 (a(n) = 1
 *            at positions A286989), else 0
 * Range:     u8 n <= 23, u16 n <= 2212, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9647 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A286987_isqrt2(seq_t m)
{
    /* floor(m*sqrt(2)) = floor(sqrt(2*m*m)) */
    seq_t x = 2 * m * m;
    seq_t lo = 0, hi = 2 * m + 1;
    while (lo < hi) {
        seq_t mid = lo + (hi - lo + 1) / 2;
        if (mid * mid <= x) lo = mid; else hi = mid - 1;
    }
    return lo;
}

static seq_t A286987_f(seq_t m)
{
    return (10 * m + A286987_isqrt2(m)) / 7;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t t = (seq_t)(n - 1);
    seq_t mc = (10 * t - A286987_isqrt2(t)) / 14;
    seq_t m = (mc > 2) ? mc - 2 : 0;
    seq_t end = mc + 3;
    for (; m <= end; m++) {
        if (A286987_f(m) == t) return 1;
    }
    return 0;
}
