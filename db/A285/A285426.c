/*
 * A285426  Numbers n such that at least two consecutive elements of the n-th
 *          row of A237591 are in increasing order.
 * https://oeis.org/A285426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m such that row m of A237591 has T(m,k) < T(m,k+1)
 *            for some k; found by direct search, T(r,k) =
 *            ceil((r+1)/k-(k+1)/2) - ceil((r+1)/(k+1)-(k+2)/2),
 *            1<=k<=floor((sqrt(8r+1)-1)/2)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static long long A285426_cdiv(long long num, long long den)
{
    if (num >= 0) return (num + den - 1) / den;
    return num / den;
}

static long long A285426_T(long long r, long long k)
{
    long long A = A285426_cdiv(2 * (r + 1) - k * (k + 1), 2 * k);
    long long B = A285426_cdiv(2 * (r + 1) - (k + 1) * (k + 2), 2 * (k + 1));
    return A - B;
}

static int A285426_has_increase(long long m)
{
    long long len = 0;
    while ((len + 1) * (len + 2) / 2 <= m) len++;
    long long k;
    for (k = 1; k < len; k++) {
        if (A285426_T(m, k) < A285426_T(m, k + 1)) return 1;
    }
    return 0;
}

seq_t a(idx_t n)
{
    long long m = 0;
    idx_t cnt = 0;
    while (cnt < n) {
        m++;
        if (A285426_has_increase(m)) cnt++;
    }
    return (seq_t)m;
}
