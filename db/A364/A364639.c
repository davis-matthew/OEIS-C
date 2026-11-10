/*
 * A364639  Irregular triangle read by rows: T(n,k) = A237591(n,k) -
 *          A237591(n-1,k).
 * https://oeis.org/A364639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) = T(r,k) = U(r,k) - U(r-1,k), the n-th entry of the rows
 *            read in order, where U = A237591, U(r,k) = ceil((r+1)/k -
 *            (k+1)/2) - ceil((r+1)/(k+1) - (k+2)/2) for k <= A003056(r), and
 *            U(r,k) = 0 for k > A003056(r) or r = 0
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int64_t A364639_cdiv(int64_t num, int64_t den)
{
    if (num >= 0)
        return (num + den - 1) / den;
    return num / den;
}

/* largest m with m(m+1)/2 <= r */
static uint64_t A364639_rowlen(uint64_t r)
{
    uint64_t lo = 0, hi = 4294967295ULL;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        if (mid * (mid + 1) / 2 <= r)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

static int64_t A364639_U(uint64_t r, uint64_t k)
{
    if (r == 0 || k == 0 || k > A364639_rowlen(r))
        return 0;
    int64_t N = (int64_t)r, K = (int64_t)k;
    int64_t A = A364639_cdiv(2 * (N + 1) - K * (K + 1), 2 * K);
    int64_t B = A364639_cdiv(2 * (N + 1) - (K + 1) * (K + 2), 2 * (K + 1));
    return A - B;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    uint64_t r = 0, cum = 0, len = 0;
    while (cum < n) {
        ++r;
        while ((len + 1) * (len + 2) / 2 <= r)
            ++len;
        cum += len;
    }
    uint64_t k = n - (cum - len);
    int64_t v = A364639_U(r, k) - A364639_U(r - 1, k);
    return (seq_t)v;
}
